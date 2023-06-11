#include "main.h"
#include "watek_glowny.h"

void mainLoopGnome(int gnomes, int dwarves, int sights, int pins)
{
    srandom(rank);
    int tag;
    int perc;
	bool ubieganie_celownik = FALSE;
	bool ubieganie_agrawka = FALSE;
	/*bool ma_agrafke = FALSE;
	bool ma_celownik = FALSE;*/
    println("Jestem nowym gnomem\n");


    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
		{
		    perc = random()%100;
		    if ( perc < 25 ) {
			    perc = random()%100;
				debug("Perc: %d", perc);
			    if ( perc < 50 || ma_agrafke) {
				    println("Rozpoczynam przetwarzanie");
				    println("Ubiegam się o sekcję krytyczną związaną z celownikiem");
				    packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
				    pkt->who = GNOME;
				    pkt->what = SIGHT;
				    ackDwarves = 0;
				    ackGnomes = 0;
				    changeState( InWantSight );
				    ubieganie_celownik = TRUE;
				    for (int i=0;i<=size-1;i++)
				    if (i!=rank)
					    sendPacket( pkt, i, REQUEST);
				    free(pkt);
			    }
			    else if(perc >=50 || ma_celownik){
				    println("Ubiegam się o sekcję krytyczną związaną z agrafką");
				    packet_t *pkt = (packet_t *) malloc(sizeof(packet_t));
				    pkt->who = GNOME;
				    pkt->what = PIN;
				    ackDwarves = 0;
				    ackGnomes = 0;
				    ubieganie_agrawka = TRUE;
				    changeState( InWantPin );
				    for (int i=0;i<=size-1;i++)
				    if (i!=rank)
					    sendPacket( pkt, i, REQUEST);
				    free(pkt);
			    }
		    }
		    else
		        //debug("Na razie czekam");
		    break;
		}
	    case InWantPin:
		{
			println("Zbieram zgody na wejście do sekcji krytycznej związanej z agrafkami");
			while ( ackDwarves + ackGnomes < dwarves + gnomes - pins);
			println("Uzbieralem wystarczajaco zgod");
			changeState(InSection);
			break;
		}
		case InWantSight:
		{
			println("Zbieram zgody na wejście do sekcji krytycznej związanej z celownikami");
			while ( ackDwarves + ackGnomes < dwarves + gnomes - sights);
			println("Uzbieralem wystarczajaco zgod");
			changeState(InSection);
			break;
		}
	    case InSection:
		{
			println("Jestem w sekcji krytycznej")

			if(ubieganie_agrawka){
				ma_agrafke = TRUE;
				ubieganie_agrawka = FALSE;
				println("Otrzymałem agrafkę                                     === A ===");
			}
			if(ubieganie_celownik){
				ma_celownik = TRUE;
				ubieganie_celownik = FALSE;
				println("Otrzymałem celownik                                    === C ===");
			}
			sleep(2);
			
			if(ma_agrafke && ma_celownik){
				ma_agrafke = FALSE;
            	ma_celownik = FALSE;
				ma_bron = TRUE;
			    println("Utworzylem bron                                        === B ===");
			    while(ma_bron);
				ubieganie_agrawka = TRUE;
				ubieganie_celownik = TRUE;
			}
			
			println("Wychodzę z sekcji krytycznej");
			changeState( InRun );
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
	bool ubieganie_bron = FALSE;
    println("Jestem nowym skrzatem\n");
    
    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
		{
			perc = random()%100;
			if ( perc < 25 ) {
				//debug("Perc: %d", perc);
				println("Rozpoczynam przetwarzanie");
				println("Ubiegam się o sekcję krytyczną związaną z brońmi")
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
		    else
		        //debug("Na razie czekam");
			break;
		}
	    case InWantWeapon:
		{
			println("Zbieram zgody na wejście do sekcji krytycznej związanej z brońmi");
			while ( ackGnomes < dwarves - ackDwarves );
			println("Zgody od gnomow (%d) musza byc rowne co najmniej liczbie skrzatow (%d) - zgody od skrzatow (%d)", ackGnomes, dwarves, ackDwarves);
			println("Uzbieralem wystarczajaco zgod");
			changeState( InSection );
			break;
		}
	    case InSection:
		{
			println("Jestem w sekcji krytycznej");
			println("Zabijam szczura                                        === SZ ===");
			sleep(5);
			println("Wychodzę z sekcji krytycznej związanej z brońmi");
	
			changeState( InRun );
			break;
		}
	    default: 
		{break;}
        }
        sleep(SEC_IN_STATE);
    }
}
