# Proiect Sincretic - Rețele de calculatoare

## **Cerință**
Să se realizeze o aplicație client-server pentru transferul de fișiere. Serverul va avea setat un director de unde se vor prelua fișierele. Clientul va cere lista fișierelor din director cu ajutorul comenzii ls și va prelua un fișier cu ajutorul comenzii get nume_fisier. Protocolul de transport folosit va fi TCP. Serverulva fi iterativ.

## **Manual Utilizare**
## ***Client***
### Compilare: `gcc client.c -o client`

### Utilizare: 
- `./client [help]` - afișează pagina de help
- `./client ls` - afișează lista de fișiere de pe server
- `./client get <filename>` - preia un fișier de pe server pe local


## ***Server***
### Compilare: `gcc client.c -o client [-D <src_dir>]`
(Directorul implicit este "server_source")
### Utilizare:
- `./server`
- Pentru închidere, se utilizează SIGINT (`Ctrl+C`)