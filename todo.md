- [X] fifo thread-safe
- [X] struct compartilhado (stats)
    - [X] Implementação de memória compartilhada
    - [X] Funções de escrita
    - [X] Função de leitura
- [.] pipe entre processos
  - [X] primeiro ver se realmente tem que ser ao mesmo tempo
    - não vai precisar
- [X] main instânciar os 3 processos
- [X] argumentos do main
    - [X] definir opções
    - [X] processar elas
    - [X] Validar a existência dos arquivos de input

- [ ] compactador (processo)
  - [ ] criar as 3 threads
  - [ ] atualizar o status compartilhado 
- [ ] descompactador (processo)
- [ ] apresentação dos dados (Monitor)
    - ler dos stats compartilhados


- [X] algoritmo de tokenização (thread de leitura)
    - [X] Separar os espaços e pontuação
    - [X] Aceitar acentos
- [ ] algoritmo de compactação aprimorado
    - [ ] quantidade do mesmo token
      - o primeiro indice do dicionario é o byte que diz quantas vezes o token anterior vai se repetir
    - se o token é um token novo
        - armazena o token e define o byte dele
    - seleciona o byte do token
    - envia o byte para o gravador (thread de gravação)
    - grava o dicionário no arquivo compactado
- [ ] descompactador aprimorado
- [ ] lendo arquivo
    - le o dicionário do arquivo
    - até o final do arquivo
    - le o próximo byte
    - converte o byte em token usando o dicionário
    - envia para o gravador


# Dificuldades:
- Qual IPC usar para cada caso
  - mutex
  - shm
- Acentos na tokenização (são um byte separado)
- Não usar o cariage return (0xC)
  - ele era inserido automaticamente
