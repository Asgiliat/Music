#ifndef SONGVIEW_H
#define SONGVIEW_H

#include "songdata.h"
#include <QWidget>

void bildSongView(SongData::Party& party, QWidget& baseWidget);

class Visitor
{
public:
    virtual ~Visitor() = default;
    virtual void visitChord(const SongData::Chord& chord) = 0;
    virtual void visitBarLine(const SongData::BarLine& line) = 0;
    virtual void visitBarSize(const SongData::BarSize& size) = 0;
};

#endif // SONGVIEW_H
