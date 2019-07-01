#include "songview.h"
#include <QLabel>

class StuffViewVisitor : public Visitor
{
public:
    StuffViewVisitor(SongData::ClefType clefType, QWidget& baseWidget);

    void addClef();

    virtual void visitChord(const SongData::Chord& chord) override;
    virtual void visitBarLine(const SongData::BarLine& line) override;
    virtual void visitBarSize(const SongData::BarSize& size) override;

private:
    SongData::ClefType clefType;
    QWidget&           baseWidget;

    // Base Geometry
          int32_t left   = 20;
    const int32_t top    = 20;
    const int32_t width  = 20;
    const int32_t height = 50;
};

const QString style = "font-family:Bravura; font-size:14pt;";

constexpr int32_t pixBetweenLines = 4;
constexpr int32_t subLineSize = pixBetweenLines / 2;

constexpr char32_t ClefCodes[static_cast<int32_t>(SongData::ClefType::Count)] = {0xE050, 0xE062};
constexpr int32_t  ClefOffset[static_cast<int32_t>(SongData::ClefType::Count)] = {8, -2};

constexpr char32_t ZeroCharacterCode = 0xE080;
constexpr char32_t BarLineEnd        = 0xE032;

constexpr char32_t noteDurationMap[9] = {0, 0xE1D2, 0xE1D3, 0, 0xE1D5, 0, 0, 0, 0xE1D7 };

constexpr char32_t sharpCode = 0xE262;

constexpr char32_t flagsCodes[static_cast<int32_t>(SongData::Flags::Count)] = {0, 0, 0xE4B3, 0xE4A0, 0};

enum class Alteration
{
    Sharp,
    Flat,
    Natural,
    None,
};

static int32_t getGClefHeightOffsetAndAlteration(uint8_t note, Alteration& alter)
{
    int32_t halfToneDelta = int32_t(note) - int32_t(SongData::ClefNote[0]);

    // 7 - C
    // 6 - #C
    // 5 - D
    // 4 - #D
    // 3 - E
    // 2 - F
    // 1 - #F
    // 0 - G
    // -1 - #G
    // -2 - A
    // -3 - #A
    // -4 - B
    // -5 - C
    // -6 - C#

    switch(halfToneDelta)
    {
    case -2: // F
        alter = Alteration::None;
        return 1 * subLineSize;
    case -1: // #F
        alter = Alteration::Sharp;
        return 1 * subLineSize;
    case 0:  // G
        alter = Alteration::None;
        return 0;
    case 1: // #G
        alter = Alteration::Sharp;
        return 0;
    case 2: // A
        alter = Alteration::None;
        return -1 * subLineSize;

    default:
        Q_ASSERT(false); // Not implemented;
        return 0;
    }
}

static void createLabel(QChar text, int32_t left, int32_t top, int32_t width, int32_t height, QWidget& baseWidget)
{
    QLabel* clefLabel = new QLabel(text, &baseWidget);
    clefLabel->setStyleSheet(style);
    clefLabel->setGeometry(left, top, width, height);
    clefLabel->show();
}

void bildSongView(SongData::Party& party, QWidget& baseWidget)
{
    StuffViewVisitor stuffViewVisitor(party.clefType, baseWidget);

    stuffViewVisitor.addClef();

    for(auto& element : party.elements)
        element->acceptVisitor(stuffViewVisitor);
}

StuffViewVisitor::StuffViewVisitor(SongData::ClefType clefType, QWidget& baseWidget)
 : clefType(clefType)
 , baseWidget(baseWidget)
{
}

void StuffViewVisitor::addClef()
{
    createLabel(QChar(ClefCodes[static_cast<int32_t>(clefType)]), left, top + ClefOffset[static_cast<int32_t>(clefType)], width, height, baseWidget);
    left += width;
}

void StuffViewVisitor::visitChord(const SongData::Chord& chord)
{
    Q_ASSERT(chord.getDuration().denominator <= 8);
    const char32_t durationUTFCode = noteDurationMap[chord.getDuration().denominator];
    Q_ASSERT(durationUTFCode != 0);
    const QChar durationCode(durationUTFCode);

    const int32_t noteToClefOffset = ClefOffset[static_cast<int32_t>(clefType)];

    if(clefType == SongData::ClefType::GClef)
    {
        for(size_t id=0; id<chord.getNoteAmount(); ++id)
        {
            Alteration alter;
            const int32_t toneOffset = getGClefHeightOffsetAndAlteration(chord.getNote(id), alter);

            if(alter == Alteration::Sharp)
            {
                createLabel(QChar(sharpCode), left, top + noteToClefOffset + toneOffset, width, height, baseWidget);
                left += 6;
            }
            else if(alter != Alteration::None)
            {
                Q_ASSERT(false);
            }

            createLabel(durationCode, left, top + noteToClefOffset + toneOffset, width, height, baseWidget);
        }
    }
    else
    {
        Q_ASSERT(false);
    }

    int32_t flagTop = top + 20;
    for(auto& flag : chord.getFlags())
    {
        const char32_t flagUTFCode = flagsCodes[uint32_t(flag)];
        Q_ASSERT(flagUTFCode != 0);
        const QChar flagCode(flagUTFCode);

        createLabel(flagCode, left, flagTop, width, height, baseWidget);
        flagTop += 8;
    }

    left += width;
}

void StuffViewVisitor::visitBarLine(const SongData::BarLine& line)
{
    // Not implemented cases:
    Q_ASSERT(line.isRepeatOpen() == false);
    Q_ASSERT(line.isRepeatClose() == false);

    if(line.isEndOfParty())
        createLabel(QChar(BarLineEnd), left, top+13, width, height, baseWidget);
}

void StuffViewVisitor::visitBarSize(const SongData::BarSize& barSize)
{
    QChar nominatorCode(ZeroCharacterCode + barSize.getNominator());
    QChar denominatorCode(ZeroCharacterCode + barSize.getDenominator());

    createLabel(nominatorCode,   left, top-2, width, height, baseWidget);
    createLabel(denominatorCode, left, top+8, width, height, baseWidget);
    left += width;
}
