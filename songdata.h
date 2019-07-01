#ifndef TRACKDATA_H
#define TRACKDATA_H

#include <stdint.h>
#include <vector>

// It uses Piano-keyboard range to index notes

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

   Duration() : denominator(4), dot(false) {}
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
};

class Chord : public BaseElement
{
public:
   uint8_t            notes[maxNotesPerChord];
   Duration           duration;
   std::vector<Flags> flags;

   Chord() : notes{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } {}
};

class BarLine : public BaseElement
{
public:
   bool repeatOpen;
   bool repeatClose;
   bool endOfParty;

   BarLine(bool ro, bool rc, bool en) : repeatOpen(ro), repeatClose(rc), endOfParty(en) {}
};

class BarSize : public BaseElement
{
public:
   uint8_t nominator;
   uint8_t denominator;

   BarSize(uint8_t nom, uint8_t denom) : nominator(nom), denominator(denom) {}
};

// struct KeySignature : BaseElement
// {
// };


struct Party
{
   ClefType                  clefType;
   std::vector<BaseElement*> elements;
};

}


#endif // TRACKDATA_H
