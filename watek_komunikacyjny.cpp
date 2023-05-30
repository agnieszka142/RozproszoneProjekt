#include "main.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startComGnome(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while ( stan!=InFinish ) {
	debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        
        switch ( status.MPI_TAG ) {
	    case REQUEST:
            if(pakiet.what == PIN){
                if(stan == InWantPin && pakiet.ts > vClock){
                    if(pakiet.who == DWARF){
                        ackDwarves++;
                    }
                    if(pakiet.who == GNOME){
                        ackGnomes++;
                    }
                }
                if(stan != InWantPin){
                    sendPacket( 0, status.MPI_SOURCE, ACK );
                }
            }
            if(pakiet.what == SIGHT){
                if(stan == InWantSight && pakiet.ts > vClock){
                    if(pakiet.who == DWARF){
                        ackDwarves++;
                    }
                    if(pakiet.who == GNOME){
                        ackGnomes++;
                    }
                }
                if(stan != InWantSight){
                    sendPacket( 0, status.MPI_SOURCE, ACK );
                }
            }
            if(pakiet.what == WEAPON){
                if(bronie > 0){
                    sendPacket( 0, status.MPI_SOURCE, ACK );
                    bronie--;
                }
            }
	    break;
	    case ACK: 
            debug("Dostałem ACK od %d, mam już %d", status.MPI_SOURCE, ackDwarves+ackGnomes);
	        if(pakiet.who == DWARF){
                ackDwarves++;
            }
            if(pakiet.who == GNOME){
                ackGnomes++;
            }
	    break;
	    default:
	    break;
        }
        pthread_mutex_lock(&vClockMut);
        if (vClock < pakiet.ts){vClock = pakiet.ts;}
        vClock++;
        pthread_mutex_unlock( &vClockMut );
    }
}

void *startComDwarf(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    std::vector<int> oczekujace_bron;
	std::vector<int> oczekujace_agrafki;
	std::vector<int> oczekujace_celowniki;
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while ( stan!=InFinish ) switch ( status.MPI_TAG ) {
	    case REQUEST:
            if(pakiet.what == PIN){
                if(stan == InWantWeapon){
                    sendPacket( 0, status.MPI_SOURCE, ACK );
                }else{
                    oczekujace_agrafki.push_back(pakiet.src);
                }
            }
            if(pakiet.what == SIGHT){
                if(stan == InWantWeapon){
                    sendPacket( 0, status.MPI_SOURCE, ACK );
                }else{
                    oczekujace_celowniki.push_back(pakiet.src);
                }
            }
            if(pakiet.what == WEAPON){
                if(stan == InWantWeapon && pakiet.ts > vClock){
                    if(pakiet.who == DWARF){
                        ackDwarves++;
                    }
                    if(pakiet.who == GNOME){
                        ackGnomes++;
                    }
                }
                if(stan != InWantWeapon){
                    sendPacket( 0, status.MPI_SOURCE, ACK );
                }
            }
	    break;
	    case ACK: 
            debug("Dostałem ACK od %d, mam już %d", status.MPI_SOURCE, ackDwarves+ackGnomes);
	        if(pakiet.who == DWARF){
                ackDwarves++;
            }
            if(pakiet.who == GNOME){
                ackGnomes++;
            }
	    break;
	    default:
	    break;
        }
        pthread_mutex_lock(&vClockMut);
        if (vClock < pakiet.ts){vClock = pakiet.ts;}
        vClock++;
        pthread_mutex_unlock( &vClockMut );
}
