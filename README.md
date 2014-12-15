# Gain Filter

Este filtro tem como função alterar o brilho e contraste da imagem carregada.Foram implementadas duas versões, a serial e a paralela, afim de comparar os seus desempenhos de processamento.

--------------------------------------------------------------
## Versão Serial

Inicialmente, a imagem é carregada na linguagem na C++. No bloco __asm, subtraimos threshold dos três canais e para fazer a alteração do contraste, multiplicamos estes canais por um fator de contraste e adicionamos o threshold de volta.
Para o brilho, apenas adicionamos o fator de brilho em cada um dos canais. Após estes dois processos, tivemos que certificar que o valor esta entre 0 e a profundidade de cores.

--------------------------------------------------------------

## Versão Paralela (Instruções SSE2)

O algoritimo utilizado para as alterações de bilho e contraste será basicamento o mesmo, porém, neste versão iremos ter um ganho de desempenho devido as instruções utilizadas (SIMD) que realiza 16 operações por vez em uma única instrução, diferente da versão serial que realiza uma operação por instrução. Devido a isso, temos que verificar se a quantidade de pixels da imagem é multiplo de 16, caso contrário, adicionamos a quantidade de pixels necessária para obtermos este múltiplo. No momento da escrita, estes pixels extras serão ignorados.

---------------------------------------------------------------

## Problemas e Resultados

Verificamos através de testes com diversas imagens, que obtivemos os resultados esperados na versão serial. Entretanto, na versão paralela, ocorreram problemas no contraste devido a soma saturada, pois utilizamos esta soma com 16 inteiros de 8 bits sem sinal (PADDIUSB), isso acarreta em um problema para a situação em que o valor do canal for menor que 128 (p.e 110), ao final da aplicação de contraste ele será erroneamente substítuido por 128. Com isso, a imagem resultante será diferente da versão serial no que diz respeito ao contraste apenas. Não conseguimos obter uma solução para este problema a tempo.

Em relação ao processamento, em todos os casos de testes a versão paralela foi mais rápida (cerca da metade do tempo).

## Gráfico do processamento:
![](http://imagizer.imageshack.us/v2/1280x1024q90/901/0gO5SV.png)

## Resultados das imagens:

Beach original:
![](http://imagizer.imageshack.us/v2/1280x1024q90/674/q5Bz59.jpg)


Beach serial:
![](http://imagizer.imageshack.us/v2/1280x1024q90/537/Qwrtaz.jpg)


Beach paralela:
![](http://imagizer.imageshack.us/v2/1280x1024q90/911/Q6Uu33.jpg)


Lorikeet original:

![](http://imagizer.imageshack.us/v2/1280x1024q90/661/U19CAY.jpg)


Lorikeet serial:

![](http://imagizer.imageshack.us/v2/1280x1024q90/540/STpfAv.jpg)


Lorikeet paralela:

![](http://imagizer.imageshack.us/v2/1280x1024q90/913/RP1cOk.jpg)


Torre original:
![](http://imagizer.imageshack.us/v2/1280x1024q90/674/qe5dGl.jpg)


Torre serial:
![](http://imagizer.imageshack.us/v2/1280x1024q90/911/HY1DhJ.jpg)


Torre paralela:
![](http://imagizer.imageshack.us/v2/1280x1024q90/905/lVSgZA.jpg)
