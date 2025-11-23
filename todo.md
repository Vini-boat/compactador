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

- [X] compactador (processo)
  - [X] criar as 3 threads
  - [X] atualizar o status compartilhado 
- [X] descompactador (processo)
- [X] apresentação dos dados (Monitor)
    - ler dos stats compartilhados


- [X] algoritmo de tokenização (thread de leitura)
    - [X] Separar os espaços e pontuação
    - [X] Aceitar acentos


# Tópicos da Apresentação
- Excalidraw
  - Como os processos trocam informações.
    - fifo
    - shm
  - Como as threads trabalham em conjunto para compactar.
- Compilar
- Executar
  - benchmark entrada.txt
  - benchmark entrada2.txt
  - benchmark entrada5.txt
    - mostrar arvore de processos  
- Dicionário ampliado
- Quais dificuldades técnicas foram enfrentadas e como foram resolvidas. 

# Dificuldades:
- Qual IPC usar para cada caso
  - mutex
  - shared memory
- Dicionário dinâmico não vale a pena para troca simples de token por byte
  - no final o arquivo fica maior
- Acentos na tokenização (são um byte separado)
- Não usar o cariage return (0xC)
  - ele era inserido automaticamente




