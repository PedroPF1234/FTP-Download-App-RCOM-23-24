### Rede de Computadores

## 2º Trabalho Laboratorial

Este projeto foi realizado no âmbito da unidade curricular de Redes de
Computadores da Faculdade de Engenharia da Universidade do Porto, com o objetivo de
configurar uma rede de computadores e implementar a funcionalidade de download, usando 
FTP, através da rede estabelecida. Este repositório apenas incluí o código fonte da aplicação
de download utilizada.

## Arquitetura

A aplicação desenvolvida efetua o descarregamento de um ficheiro usando o
protocolo FTP. Para este propósito, foram consultadas a norma RFC959, que descreve o
funcionamento e a arquitetura do protocolo FTP, bem como a norma RFC1738, que trata
das diferentes componentes dos endereços URL.

Primeiramente é feito o parsing do URL fornecido como argumento através da
função [parse_url(char\* host, struct URL \*url)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L11),
que implementa uma máquina de estados
para atingir este objetivo. Esta função analisa um URL para extrair vários componentes,
como o nome do anfitrião, nome de utilizador e senha, assumindo o valor ‘anonymous’ para
ambos como default não sejam explicitados. Verifica se o URL começa com "ftp://", e depois
processa a string para identificar diferentes partes do URL. De seguida, é feita a conexão ao
servidor usando a informação extraída do URL através das funções
[open_tcp_socket_with_response(char\* ip, int port)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L124) e
[open_tcp_socket_without_response(char\* ip, int port)](https://github.com/PedroPF1234/Download-App-RCom/master/src/clientTCP.c#L154),
que permitem estabelecer uma
conexão TCP com o servidor no endereço e porta especificados, criando um socket. Ambas
as funções desempenham a mesma função, simplesmente lidam com situações diferentes.

A função [open_tcp_socket_with_response](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L124)
abre o socket na situação em que existe uma
mensagem de boas vindas/de entrada do servidor e a função
[open_tcp_socket_without_response](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L154),
tal como o nome indica lida com a abertura do socket
na ausência de uma mensagem introdutória por parte do servidor. Posto isto, é necessário
efetuar-se o login e lidar com a situação em que as credenciais não são passadas no URL.

A função [authenticate(int socket, char\* username, char\* password)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L177)
gerencia o processo de
login no servidor. Envia as credenciais necessárias (nome de utilizador e senha) através da
conexão TCP estabelecida. O próximo passo consiste em ativar o modo passivo. Nesta
situação, o cliente ficará responsável pela abertura da ligação TCP para os dados. Este
passo é realizado pela função [setPassive(int socket, int\* port, char\* ip)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L200)
, novamente com o
uso de uma máquina de estados. Seguidamente, é então feito o pedido de ficheiro
propriamente dito ao servidor através da função [request_file(int socket, char\*
resource_path)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L363)
. Esta função encarrega-se de preparar o pedido, enviá-lo e gerir a resposta
do servidor, utilizando o socket TCP já estabelecido para comunicar com o servidor. A
receção do ficheiro requisitado, por sua vez, é gerida pela função [receive_file(int socket,
char\* file)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L379)
, que lê dados do socket e escreve-os num ficheiro local, efetivamente
descarregando o ficheiro do servidor. Finalmente terminada a transferência procede-se ao
fecho do socket, através da função [close_tcp_socket(int socket)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L413). Este é um passo essencial
para terminar adequadamente uma conexão TCP para evitar perdas de recursos.

Paralelamente também foram elaboradas as funções auxiliares [send_string(int
socket, char \*buf)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L402)
e [readResponse(int socket, char \*response)](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L284)
. A função [send_string](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L402) envia
uma string para o servidor. É usada para enviar comandos ou dados através da conexão
TCP. A função [read_response](https://github.com/PedroPF1234/Download-App-RCom/blob/master/src/clientTCP.c#L284) tem a responsabilidade de lidar com as respostas do
servidor, verificar a sua validade e escrevê-las na consola.

Estas funções, juntas, implementam um cliente FTP (Protocolo de Transferência de
Ficheiros) básico, que pode conectar-se a um servidor FTP, fazer login, mudar para o modo
passivo, receber ficheiros e enviar comandos.

A aplicação foi testada procedendo-se ao download de vários ficheiros de diversos
tamanhos.
