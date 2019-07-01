#include "songdata.h"
#include "songview.h"

namespace SongData
{

void Chord::acceptVisitor(Visitor& vis)
{
    vis.visitChord(*this);
}

void BarLine::acceptVisitor(Visitor &vis)
{
    vis.visitBarLine(*this);
}

void BarSize::acceptVisitor(Visitor &vis)
{
    vis.visitBarSize(*this);
}

}
