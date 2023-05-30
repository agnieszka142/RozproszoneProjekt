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


    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
		{perc = random()%100;
		if ( perc < 25 ) {
			perc = random()%100;
			if ( perc < 50 || ma_agrafke) {
				debug("Perc: %d", perc);
				println("Ubiegam się o sekcję krytyczną związaną z celownikiem");
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
				pkt->who = GNOME;
				pkt->what = PIN;
				ackDwarves = 0;
				ackGnomes = 0;
				ubieganie_celownik = TRUE;
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
					sendPacket( pkt, i, REQUEST);
				changeState( InWantSight );
				free(pkt);
			}
			if(perc >=50 || ma_celownik){
				debug("Perc: %d", perc);
				println("Ubiegam się o sekcję krytyczną związaną z agrafką");
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
				pkt->who = GNOME;
				pkt->what = SIGHT;
				ackDwarves = 0;
				ackGnomes = 0;
				ubieganie_agrawka = TRUE;
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
		{
			println("Czekam na wejście do sekcji krytycznej związanej z agrafkami");
			if ( ackDwarves + ackGnomes >= dwarves + gnomes - pins){
				changeState(InSection);
			}
			break;
		}
		case InWantSight:
		{
			println("Czekam na wejście do sekcji krytycznej związanej z celownikami")
			if ( ackDwarves + ackGnomes >= dwarves + gnomes - sights) 
				changeState(InSection);
			break;
		}
	    case InSection:
		{
			println("Jestem w sekcji krytycznej")
			sleep(5);
			//if ( perc < 25 ) {
				debug("Perc: %d", perc);
				println("Wychodzę z sekcji krytycznej");


				if(ubieganie_agrawka = TRUE){
					ma_agrafke = TRUE;
					ubieganie_agrawka = FALSE;
					println("Otrzymałem agrafkę")	
				}
				if(ubieganie_celownik = TRUE){
					ma_celownik = TRUE;
					ubieganie_celownik = FALSE;
					println("Otrzymałem celownik")
				}
				if(ma_agrafke && ma_celownik){
						ma_agrafke = FALSE;
                    	ma_celownik = FALSE;
						bronie++;
						ubieganie_agrawka = TRUE;
						ubieganie_celownik = TRUE;
				}
				changeState( InRun );
			//}
			break;
		}
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

    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
		{
			perc = random()%100;
			if ( perc < 25 ) {
				debug("Perc: %d", perc);
				println("Ubiegam się o sekcję krytyczną związaną z brońmi")
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
				pkt->who = DWARF;
				pkt->what = WEAPON;
				ackDwarves = 0;
				ackGnomes = 0;
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
					sendPacket( pkt, i, REQUEST);
				changeState( InWantWeapon );
				free(pkt);
			}
			debug("Skończyłem myśleć");
			break;
		}
	    case InWantWeapon:
		{
			println("Czekam na wejście do sekcji krytycznej związanej z brońmi")
			if ( ackDwarves - dwarves >= ackGnomes ) 
				changeState(InSection);
			break;
		}
	    case InSection:
		{
			println("Jestem w sekcji krytycznej")
			sleep(5);
			//if ( perc < 25 ) {
			debug("Perc: %d", perc);
			println("Wychodzę z sekcji krytycznej związanej z brońmi");
	
			changeState( InRun );
			//}
			break;
		}
	    default: 
		{break;}
        }
        sleep(SEC_IN_STATE);
    }
}
