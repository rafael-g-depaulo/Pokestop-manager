## Problema de concorrência a ser resolvido: **Pocket Monsters' Daycare Management**

N treinadores tem X (X <= N) conjuntos de Y monstros de tipos diferentes (dragões, hidras, pernilongos, etc.). Eles querem botar os monstros na creche, para se livrar deles.

Tem uma creche para cada tipo de monstro, e depois de ficar na creche pelo tempo necessário, os monstros crescem e voltam pra natureza. Cada creche tem um limite de monstros que pode cuidar ao mesmo tempo, e cada creche tem um limite de treinadores que podem estar depositando seus monstros ao mesmo tempo.

Cada treinador demora 1s para depositar um monstro, e cada monstro demora entre 3 e 10 segundos para amadurecer e sair da creche (depende do tipo do monstro).

Cada treinador escolhe aleatoriamente entre 1 e 5 monstros de um tipo que ele ainda tenha para depositar na creche por vez. Se a creche escolhida não tem espaço para os monstros que ele escolheu depositar, o treinador passa 2s repensando a sua escolha e escolhe outro tipo de monstros e quantidade para depositar. Se necessário, ele espera na fila para poder depositar seus monstros.
Uma vez terminado de depositar seus monstros, ele descansa por 5s, e repete.

Quando um treinador terminar de depositar todos os seus monstros, ele para. Quando todos os treinadores se livrarem dos seus monstros, a situação está resolvida.


### Solução
Inicialmente será feita uma solução para uma situação com somente um tipo de monstro (4 por treinador), e 3 treinadores, com a única creche tendo espaço para 2 treinadores por vez, e 4 monstros por vez