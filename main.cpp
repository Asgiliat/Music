#include "mainwindow.h"
#include "songdata.h"
#include "songview.h"
#include <QApplication>
#include <QLabel>

void fillParty(SongData::Party& party)
{
   using namespace SongData;

   party.clefType = ClefType::GClef;
   party.elements.push_back(new BarSize(4, 4));

   Chord* c =  new Chord;
   c->notes[0] = 42;           // #F, 6+3*12
   c->flags.push_back(Flags::StacattoTenuto);
   party.elements.push_back(c);

   c = new Chord;
   c->notes[0] = 45;           // A,  9+3*12
   c->flags.push_back(Flags::AccentuatedNote);
   party.elements.push_back(c);

   c = new Chord;
   c->notes[0] = 43;           // G, 7+3*12
   c->duration.denominator = 2;
   party.elements.push_back(c);

   party.elements.push_back(new BarLine(false, false, true));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SongData::Party party;
    fillParty(party);

    bildSongView(party, w);

    return a.exec();
}
