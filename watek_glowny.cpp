#include "main.h"
#include "watek_glowny.h"

void mainLoopGnome(int gnomes, int dwarves, int sights, int pins)
{
    srandom(rank);
    int tag;
    int perc;
	bool ubieganie_celownik;
	bool ubieganie_agrawka;
	bool ma_agrafke;
	bool ma_celownik;
	int bronie;


    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
		{perc = random()%100;
		if ( perc < 25 ) {
			perc = random()%100;
			if ( perc < 50 ) {
				debug("Perc: %d", perc);
				println("Ubiegam się o sekcję krytyczną związaną z celownikiem")
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
				pkt->data = SIGHT;
				ackCount = 0;
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
					sendPacket( pkt, i, REQUEST);
				changeState( InWantSight );
				free(pkt);
			}else{
				debug("Perc: %d", perc);
				println("Ubiegam się o sekcję krytyczną związaną z agrafką")
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
				pkt->data = PIN;
				ackCount = 0;
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
					sendPacket( pkt, i, REQUEST);
				changeState( InWantPin );
				free(pkt);
			}
		}
		debug("Skończyłem myśleć");
		break;}
	    case InWantPin:
		{println("Czekam na wejście do sekcji krytycznej związanej z agrafkami")
		// tutaj zapewne jakiś muteks albo zmienna warunkowa
		// bo aktywne czekanie jest BUE
		if ( ackCount == dwarves + gnomes - pins) 
		    changeState(InSection);
		break;}
		case InWantSight:
		{println("Czekam na wejście do sekcji krytycznej związanej z celownikami")
		// tutaj zapewne jakiś muteks albo zmienna warunkowa
		// bo aktywne czekanie jest BUE
		if ( ackCount == dwarves + gnomes - sights) 
		    changeState(InSection);
		break;}
	    case InSection:
		{// tutaj zapewne jakiś muteks albo zmienna warunkowa
		println("Jestem w sekcji krytycznej")
		    sleep(5);
		//if ( perc < 25 ) {
		    debug("Perc: %d", perc);
		    println("Wychodzę z sekcji krytyczneh")
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
		    pkt->data = perc;
		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			    sendPacket( pkt, (rank+1)%size, RELEASE);
		    changeState( InRun );
		    free(pkt);
		//}
		break;}
	    default: 
		{break;}
        }
        sleep(SEC_IN_STATE);
    }
}

void mainLoopDwarf(int gnomes, int dwarves)
{
    srandom(rank);
    int tag;
    int perc;
	bool ubieganie_bron;
	std::vector<int> oczekujace_bron;
	std::vector<int> oczekujace_agrafki;
	std::vector<int> oczekujace_celowniki;

    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
		{perc = random()%100;
		if ( perc < 25 ) {
		    debug("Perc: %d", perc);
		    println("Ubiegam się o sekcję krytyczną")
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
		    pkt->data = perc;
		    ackCount = 0;
		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			    sendPacket( pkt, i, REQUEST);
		    changeState( InWant ); // w VI naciśnij ctrl-] na nazwie funkcji, ctrl+^ żeby wrócić
					   // :w żeby zapisać, jeżeli narzeka że w pliku są zmiany
					   // ewentualnie wciśnij ctrl+w ] (trzymasz ctrl i potem najpierw w, potem ]
					   // między okienkami skaczesz ctrl+w i strzałki, albo ctrl+ww
					   // okienko zamyka się :q
					   // ZOB. regułę tags: w Makefile (naciśnij gf gdy kursor jest na nazwie pliku)
		    free(pkt);
		} // a skoro już jesteśmy przy komendach vi, najedź kursorem na } i wciśnij %  (niestety głupieje przy komentarzach :( )
		debug("Skończyłem myśleć");
		break;}
	    case InWant:
		{println("Czekam na wejście do sekcji krytycznej")
		// tutaj zapewne jakiś muteks albo zmienna warunkowa
		// bo aktywne czekanie jest BUE
		if ( ackCount == size - 1) 
		    changeState(InSection);
		break;}
	    case InSection:
		{// tutaj zapewne jakiś muteks albo zmienna warunkowa
		println("Jestem w sekcji krytycznej")
		    sleep(5);
		//if ( perc < 25 ) {
		    debug("Perc: %d", perc);
		    println("Wychodzę z sekcji krytyczneh")
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
		    pkt->data = perc;
		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			    sendPacket( pkt, (rank+1)%size, RELEASE);
		    changeState( InRun );
		    free(pkt);
		//}
		break;}
	    default: 
		{break;}
        }
        sleep(SEC_IN_STATE);
    }
}
