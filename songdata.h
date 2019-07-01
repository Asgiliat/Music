#ifndef TRACKDATA_H
#define TRACKDATA_H

#include <stdint.h>
#include <vector>
#include <memory>
#include <QtGlobal>

// It uses Piano-keyboard range to index notes

class Visitor;

namespace SongData
{

enum class ClefType : int32_t
{
  GClef,
  FClef,

  Count,
};

constexpr size_t maxNotesPerChord = 10;
constexpr uint8_t ClefNote[(int32_t)ClefType::Count] = {43, 29};     // 7 + 3*12, 5 + 2*12

struct Duration
{
   uint8_t denominator;               // Note duration is 1/denominator
   bool    dot;

   Duration(uint8_t denominator = 4) : denominator(denominator), dot(false) {}
};

enum class Flags : uint32_t
{
   Stacatto,
   Tenuto,
   StacattoTenuto,
   AccentuatedNote,
   MutedNode,
   PalmMute,
   GhostNote,
   // etc.

   Count,
};

class BaseElement
{
public:
   virtual ~BaseElement(){}
   virtual void acceptVisitor(Visitor&) {}
};

class Chord : public BaseElement
{
public:
    virtual void acceptVisitor(Visitor& vis);
    Chord(Duration dur = Duration()) : notes{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, noteAmount(0), duration(dur) {}

    void addNote(uint8_t note)
    {
        Q_ASSERT(noteAmount < maxNotesPerChord);
        notes[noteAmount++] = note;
    }

    void addFlag(Flags flag)
    {
        flags.push_back(flag);
    }

    size_t getNoteAmount() const { return noteAmount; }
    uint8_t getNote(size_t index) const
    {
        Q_ASSERT(index < maxNotesPerChord);
        return notes[index];
    }

    Duration getDuration() const { return duration; }

    const std::vector<Flags>& getFlags() const { return flags; }

private:
   uint8_t            notes[maxNotesPerChord];
   size_t             noteAmount;
   Duration           duration;
   std::vector<Flags> flags;
};

class BarLine : public BaseElement
{
public:
    BarLine(bool ro, bool rc, bool en) : repeatOpen(ro), repeatClose(rc), endOfParty(en) {}

    virtual void acceptVisitor(Visitor& vis);

    bool isEndOfParty() const { return endOfParty; }
    bool isRepeatOpen() const { return repeatOpen; }
    bool isRepeatClose() const { return repeatClose; }

private:
   bool repeatOpen;
   bool repeatClose;
   bool endOfParty;
};

class BarSize : public BaseElement
{
public:
   BarSize(uint8_t nom, uint8_t denom) : nominator(nom), denominator(denom) {}

   virtual void acceptVisitor(Visitor& vis);

   uint8_t getNominator() const { return nominator; }
   uint8_t getDenominator() const { return denominator; }

private:
   uint8_t nominator;
   uint8_t denominator;
};

struct Party
{
   ClefType                                  clefType;
   std::vector<std::unique_ptr<BaseElement>> elements;
};

}

#endif // TRACKDATA_H
