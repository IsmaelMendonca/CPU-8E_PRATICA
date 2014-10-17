// Biblioteca de interface para display de LCD
#include <LiquidCrystal.h>
// Biblioteca para interface para memória EEPROM do Arduino
#include <EEPROM.h>

// Definição de constantes a serem utilizadas
#define POSICAO_INICIAL_EEPROM 0
#define POSICAO_FINAL_EEPROM 255
#define VALOR_ZERO 0
#define TEMPO_0_MILISEGUNDOS 0
#define TEMPO_10_MILISEGUNDOS  10
#define TEMPO_250_MILISEGUNDOS 250
#define TEMPO_1000_MILISEGUNDOS  1000
#define ENDERECO_INICIAL_CONTADOR_PROGRAMA 0x00
#define ENDERECO_FINAL_CONTADOR_PROGRAMA 0xFF
#define ZERO_BINARIO 0b00000000
#define ZERO_HEXADECIMAL 0x00
#define DEZESSEIS_HEXADECIMAL 0x10
#define INSTRUCAO_UMA_PALAVRA 0
#define INSTRUCAO_DUAS_PALAVRAS 1
#define ENDERECAMENTO_IMEDIATO 0
#define ENDERECAMENTO_DIRETO 1
#define INATIVO 0
#define ATIVO 1

int tempoMinimoPressionamentoBotao = TEMPO_250_MILISEGUNDOS;
long tempoSemPressionarBotaoPassoSimples = TEMPO_0_MILISEGUNDOS;
long tempoSemPressionarBotaoCicloSimples = TEMPO_0_MILISEGUNDOS;
bool botaoPassoSimplesEmPressionamento = INATIVO;
bool botaoCicloSimplesEmPressionamento = INATIVO;


// Definicao dos codigos de operacao em base hexadecimal
const byte HLT_IMEDIATO_UMA_PALAVRA = 0x00;
const byte NOP_IMEDIATO_UMA_PALAVRA = 0x01;
const byte NOT_IMEDIATO_UMA_PALAVRA = 0x02;
const byte RET_IMEDIATO_UMA_PALAVRA = 0x03;
const byte JMP_DIRETO_DUAS_PALAVRAS = 0xC4;
const byte JEQ_DIRETO_DUAS_PALAVRAS = 0xC5;
const byte JGT_DIRETO_DUAS_PALAVRAS = 0xC6;
const byte JGE_DIRETO_DUAS_PALAVRAS = 0xC7;
const byte JCY_DIRETO_DUAS_PALAVRAS = 0xC8;
const byte CAL_DIRETO_DUAS_PALAVRAS = 0xC9;
const byte SHL_IMEDIATO_DUAS_PALAVRAS = 0x8A;
const byte SHL_DIRETO_DUAS_PALAVRAS = 0xCA;
const byte SHR_IMEDIATO_DUAS_PALAVRAS = 0x8B;
const byte SHR_DIRETO_DUAS_PALAVRAS = 0xCB;
const byte SRA_IMEDIATO_DUAS_PALAVRAS = 0x8C;
const byte SRA_DIRETO_DUAS_PALAVRAS = 0xCC;
const byte ROL_IMEDIATO_DUAS_PALAVRAS = 0x8D;
const byte ROL_DIRETO_DUAS_PALAVRAS = 0xCD;
const byte ROR_IMEDIATO_DUAS_PALAVRAS = 0x8E;
const byte ROR_DIRETO_DUAS_PALAVRAS = 0xCE;
const byte STO_DIRETO_DUAS_PALAVRAS = 0xD0;
const byte LOD_IMEDIATO_DUAS_PALAVRAS = 0x91;
const byte LOD_DIRETO_DUAS_PALAVRAS = 0xD1;
const byte CMP_IMEDIATO_DUAS_PALAVRAS = 0x94;
const byte CMP_DIRETO_DUAS_PALAVRAS = 0xD4;
const byte ADD_IMEDIATO_DUAS_PALAVRAS = 0x95;
const byte ADD_DIRETO_DUAS_PALAVRAS = 0xD5;
const byte SUB_IMEDIATO_DUAS_PALAVRAS = 0x96;
const byte SUB_DIRETO_DUAS_PALAVRAS = 0xD6;
const byte AND_IMEDIATO_DUAS_PALAVRAS = 0x9A;
const byte AND_DIRETO_DUAS_PALAVRAS = 0xDA;
const byte XOR_IMEDIATO_DUAS_PALAVRAS = 0x9B;
const byte XOR_DIRETO_DUAS_PALAVRAS = 0xDB;
const byte ORL_IMEDIATO_DUAS_PALAVRAS = 0x9C;
const byte ORL_DIRETO_DUAS_PALAVRAS = 0xDC;

const int BIT_0 = 0;
const int BIT_1 = 1;
const int BIT_2 = 2;
const int BIT_3 = 3;
const int BIT_4 = 4;
const int BIT_5 = 5;
const int BIT_6 = 6;
const int BIT_7 = 7;

byte BARRAMENTO_CPU_8E = ZERO_BINARIO;
byte CONTADOR_PROGRAMA = ENDERECO_INICIAL_CONTADOR_PROGRAMA;
byte REGISTRADOR_ENDERECAMENTO_MEMORIA = ZERO_BINARIO;
byte REGISTRADOR_DADOS_MEMORIA = ZERO_BINARIO;
byte PONTEIRO_PILHA = ZERO_BINARIO;
byte ACUMULADOR = ZERO_BINARIO;
byte OPERANDO_A = ZERO_BINARIO;
byte OPERANDO_B = ZERO_BINARIO;
byte REGISTRADOR_SAIDA_ULA = ZERO_BINARIO;
byte REGISTRADOR_INSTRUCOES = ZERO_BINARIO;
byte DECODIFICADOR_INSTRUCOES = ZERO_BINARIO;
bool STATUS_FLAG_ZERO = INATIVO;
bool STATUS_FLAG_NEGATIVO = INATIVO;
bool STATUS_FLAG_CARRY = INATIVO;


// Pinos para transferência de dados ao display de LCD 
int LCD_RS = 14;
int LCD_E = 15;
int LCD_D4 = 16;
int LCD_D5 = 17;
int LCD_D6 = 18;
int LCD_D7 = 19;

// Pinos para selecao de linha no primeiro demutiplexador 4 x 16
int HABILITAR_DEMUX_16 = 1;
int DEMUX_16_A = 46;
int DEMUX_16_B = 48;
int DEMUX_16_C = 50;
int DEMUX_16_D = 52;

// Pinos para selecao de linha no segundo demutiplexador 4 x 16
int HABILITAR_DEMUX_16_1 = 0;
int DEMUX_16_1_A = 2;
int DEMUX_16_1_B = 3;
int DEMUX_16_1_C = 4;
int DEMUX_16_1_D = 5;

// Pinos de modo, Cn e selecoes para o circuito integrado da ULA
int ULA_CN = 6;
int ULA_MODO = 7;
int ULA_SELECAO_0 = 8;
int ULA_SELECAO_1 = 9;
int ULA_SELECAO_2 = 10;
int ULA_SELECAO_3 = 11;

// Pinos dos botoes de passo simples e ciclo simples
int BOTAO_CICLO_SIMPLES = 12;
int BOTAO_PASSO_SIMPLES = 13;

// Pinos do barramento
int BARRAMENTO_0 = 22;
int BARRAMENTO_1 = 24;
int BARRAMENTO_2 = 26;
int BARRAMENTO_3 = 28;
int BARRAMENTO_4 = 30;
int BARRAMENTO_5 = 32;
int BARRAMENTO_6 = 34;
int BARRAMENTO_7 = 36;

// Pinos de operacoes e flags no Arduino
int OPERACAO_ARDUINO = 38;
int FLAG_ARDUINO_ZERO = 40;
int FLAG_ARDUINO_NEGATIVO = 42;
int FLAG_ARDUINO_CARRY = 44;

// Funcoes para controle de barramentos (clocks e habilitadores)
void habilitarDemux16() {
  digitalWrite(HABILITAR_DEMUX_16, LOW);
  delay(TEMPO_10_MILISEGUNDOS);
}
void desabilitarDemux16() {
  digitalWrite(HABILITAR_DEMUX_16, HIGH);
  delay(TEMPO_10_MILISEGUNDOS);
}
void habilitarDemux16_1() {
  digitalWrite(HABILITAR_DEMUX_16_1, LOW);
  delay(TEMPO_10_MILISEGUNDOS);
}
void desabilitarDemux16_1() {
  digitalWrite(HABILITAR_DEMUX_16_1, HIGH);
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorA() {
  digitalWrite(DEMUX_16_A, LOW);
  digitalWrite(DEMUX_16_B, LOW);
  digitalWrite(DEMUX_16_C, LOW);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
}
void clockRegistradorB() {
  digitalWrite(DEMUX_16_A, HIGH);
  digitalWrite(DEMUX_16_B, LOW);
  digitalWrite(DEMUX_16_C, LOW);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorSaidaUla() {
  digitalWrite(DEMUX_16_A, LOW);
  digitalWrite(DEMUX_16_B, HIGH);
  digitalWrite(DEMUX_16_C, LOW);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorBarramento() {
  digitalWrite(DEMUX_16_A, HIGH);
  digitalWrite(DEMUX_16_B, HIGH);
  digitalWrite(DEMUX_16_C, LOW);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorAcumulador() {
  digitalWrite(DEMUX_16_A, LOW);
  digitalWrite(DEMUX_16_B, LOW);
  digitalWrite(DEMUX_16_C, HIGH);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorDadosMemoria() {
  digitalWrite(DEMUX_16_A, HIGH);
  digitalWrite(DEMUX_16_B, LOW);
  digitalWrite(DEMUX_16_C, HIGH);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorEnderecamentoMemoria() {
  digitalWrite(DEMUX_16_A, LOW);
  digitalWrite(DEMUX_16_B, HIGH);
  digitalWrite(DEMUX_16_C, HIGH);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorPonteiroPilha() {
  digitalWrite(DEMUX_16_A, HIGH);
  digitalWrite(DEMUX_16_B, HIGH);
  digitalWrite(DEMUX_16_C, HIGH);
  digitalWrite(DEMUX_16_D, LOW);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorContadorPrograma() {
  digitalWrite(DEMUX_16_A, LOW);
  digitalWrite(DEMUX_16_B, LOW);
  digitalWrite(DEMUX_16_C, LOW);
  digitalWrite(DEMUX_16_D, HIGH);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorBarramentoRetorno() {
  digitalWrite(DEMUX_16_A, HIGH);
  digitalWrite(DEMUX_16_B, LOW);
  digitalWrite(DEMUX_16_C, LOW);
  digitalWrite(DEMUX_16_D, HIGH);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}
void clockRegistradorInstrucoes() {
  digitalWrite(DEMUX_16_A, LOW);
  digitalWrite(DEMUX_16_B, HIGH);
  digitalWrite(DEMUX_16_C, LOW);
  digitalWrite(DEMUX_16_D, HIGH);
  habilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
  desabilitarDemux16();
  delay(TEMPO_10_MILISEGUNDOS);
}

void habilitarRegistradorSaidaUla() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, LOW);
  digitalWrite(DEMUX_16_1_B, HIGH);
  digitalWrite(DEMUX_16_1_C, LOW);
  digitalWrite(DEMUX_16_1_D, LOW);
  habilitarDemux16_1();
}
void habilitarRegistradorBarramento() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, HIGH);
  digitalWrite(DEMUX_16_1_B, HIGH);
  digitalWrite(DEMUX_16_1_C, LOW);
  digitalWrite(DEMUX_16_1_D, LOW);
  habilitarDemux16_1();
}
void habilitarRegistradorAcumulador() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, LOW);
  digitalWrite(DEMUX_16_1_B, LOW);
  digitalWrite(DEMUX_16_1_C, HIGH);
  digitalWrite(DEMUX_16_1_D, LOW);
  habilitarDemux16_1();
}
void habilitarRegistradorDadosMemoria() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, HIGH);
  digitalWrite(DEMUX_16_1_B, LOW);
  digitalWrite(DEMUX_16_1_C, HIGH);
  digitalWrite(DEMUX_16_1_D, LOW);
  habilitarDemux16_1();
}
void habilitarRegistradorEnderecamentoMemoria() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, LOW);
  digitalWrite(DEMUX_16_1_B, HIGH);
  digitalWrite(DEMUX_16_1_C, HIGH);
  digitalWrite(DEMUX_16_1_D, LOW);
  habilitarDemux16_1();
}
void habilitarRegistradorPonteiroPilha() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, HIGH);
  digitalWrite(DEMUX_16_1_B, HIGH);
  digitalWrite(DEMUX_16_1_C, HIGH);
  digitalWrite(DEMUX_16_1_D, LOW);
  habilitarDemux16_1();
}
void habilitarRegistradorContadorPrograma() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, LOW);
  digitalWrite(DEMUX_16_1_B, LOW);
  digitalWrite(DEMUX_16_1_C, LOW);
  digitalWrite(DEMUX_16_1_D, HIGH);
  habilitarDemux16_1();
}
void habilitarRegistradorBarramentoRetorno() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, HIGH);
  digitalWrite(DEMUX_16_1_B, LOW);
  digitalWrite(DEMUX_16_1_C, LOW);
  digitalWrite(DEMUX_16_1_D, HIGH);
  habilitarDemux16_1();
}
void habilitarRegistradorInstrucoes() {
  desabilitarDemux16_1();
  delay(TEMPO_10_MILISEGUNDOS);
  digitalWrite(DEMUX_16_1_A, LOW);
  digitalWrite(DEMUX_16_1_B, HIGH);
  digitalWrite(DEMUX_16_1_C, LOW);
  digitalWrite(DEMUX_16_1_D, HIGH);
  habilitarDemux16_1();
}

// Inicializa a biblioteca LiquidCrystal com os pinos de interface
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Define que os barramentos utilizados na CPU-8E pelo Arduino serao em modo de saida
void barramentosModoSaida() {
  pinMode(BARRAMENTO_0, OUTPUT);
  pinMode(BARRAMENTO_1, OUTPUT);
  pinMode(BARRAMENTO_2, OUTPUT);
  pinMode(BARRAMENTO_3, OUTPUT);
  pinMode(BARRAMENTO_4, OUTPUT);
  pinMode(BARRAMENTO_5, OUTPUT);
  pinMode(BARRAMENTO_6, OUTPUT);
  pinMode(BARRAMENTO_7, OUTPUT);
}
// Define que os barramentos utilizados na CPU-8E pelo Arduino serao em modo de entrada
void barramentosModoEntrada() {
  pinMode(BARRAMENTO_0, INPUT);
  pinMode(BARRAMENTO_1, INPUT);
  pinMode(BARRAMENTO_2, INPUT);
  pinMode(BARRAMENTO_3, INPUT);
  pinMode(BARRAMENTO_4, INPUT);
  pinMode(BARRAMENTO_5, INPUT);
  pinMode(BARRAMENTO_6, INPUT);
  pinMode(BARRAMENTO_7, INPUT);
}
// Envia dados aos barramentos da CPU-8E
void escreveBarramentos(byte dados) {
  barramentosModoSaida();
  bool barramento[8];
  barramento[7] = dados & (1 << BIT_7);
  barramento[6] = dados & (1 << BIT_6);
  barramento[5] = dados & (1 << BIT_5);
  barramento[4] = dados & (1 << BIT_4);
  barramento[3] = dados & (1 << BIT_3);
  barramento[2] = dados & (1 << BIT_2);
  barramento[1] = dados & (1 << BIT_1);
  barramento[0] = dados & (1 << BIT_0);
  digitalWrite(BARRAMENTO_7, barramento[7]);
  delay(10);
  digitalWrite(BARRAMENTO_6, barramento[6]);
  delay(10);
  digitalWrite(BARRAMENTO_5, barramento[5]);
  delay(10);
  digitalWrite(BARRAMENTO_4, barramento[4]);
  delay(10);
  digitalWrite(BARRAMENTO_3, barramento[3]);
  delay(10);
  digitalWrite(BARRAMENTO_2, barramento[2]);
  delay(10);
  digitalWrite(BARRAMENTO_1, barramento[1]);
  delay(10);
  digitalWrite(BARRAMENTO_0, barramento[0]);
  delay(10);
}
// Le dados dos barramentos da CPU-8E
byte leBarramentos() {
  barramentosModoEntrada();
  habilitarRegistradorBarramentoRetorno();
  byte barramento;
  barramento = (digitalRead(BARRAMENTO_7) << BIT_7) | (digitalRead(BARRAMENTO_6) << BIT_6) | (digitalRead(BARRAMENTO_5) << BIT_5) | (digitalRead(BARRAMENTO_4) << BIT_4) | (digitalRead(BARRAMENTO_3) << BIT_3) | (digitalRead(BARRAMENTO_2) << BIT_2) | (digitalRead(BARRAMENTO_1) << BIT_1) | (digitalRead(BARRAMENTO_0) << BIT_0);
  
  return barramento;
}

// Mostra no display LCD mensagem de codigo de operacao invalido
void opCodeInvalido() {
  limpaDadosLcd();
  lcd.print("DI:XXX");
  lcd.setCursor(0, 1);
  lcd.print("OpCode Invalido!");
}

// Verifica se botao de passo simples foi pressionado
void aguardaPressionamentoBotaoPassoSimples(){
  while (verificaBotaoPassoSimplesPressionado() == LOW) {
  }
}
bool verificaBotaoPassoSimplesPressionado() {
  if (digitalRead(BOTAO_PASSO_SIMPLES) == HIGH) {
     tempoSemPressionarBotaoPassoSimples = millis();
     botaoPassoSimplesEmPressionamento = INATIVO;
  } else if (((millis() - tempoSemPressionarBotaoPassoSimples) > tempoMinimoPressionamentoBotao) && botaoPassoSimplesEmPressionamento == INATIVO) {
    botaoPassoSimplesEmPressionamento = ATIVO;
    return HIGH;
  }
  return LOW;
}

// Verifica se botao de ciclo simples foi pressionado
bool verificaBotaoCicloSimplesPressionado() {
  if (digitalRead(BOTAO_CICLO_SIMPLES) == HIGH) {
     tempoSemPressionarBotaoCicloSimples = millis();
  } else if (((millis() - tempoSemPressionarBotaoCicloSimples) > tempoMinimoPressionamentoBotao) && tempoSemPressionarBotaoCicloSimples != TEMPO_0_MILISEGUNDOS) {
    tempoSemPressionarBotaoCicloSimples = millis();
    return HIGH;
  }
  return LOW;
}

// Realiza a leitura de enderecamento direto na memoria
void leituraEnderecamentoDireto() {
    aguardaPressionamentoBotaoPassoSimples();
    
    REGISTRADOR_ENDERECAMENTO_MEMORIA = REGISTRADOR_DADOS_MEMORIA;
    habilitarRegistradorDadosMemoria();
    clockRegistradorEnderecamentoMemoria();
    
    aguardaPressionamentoBotaoPassoSimples();
    
    leValorRegistradorEnderecoParaRegistradorDados();
}

// Le valor do registrador de enderecos na memoria e carrega para o registrador de dados
void leValorRegistradorEnderecoParaRegistradorDados() {
      REGISTRADOR_DADOS_MEMORIA = lerEeprom(REGISTRADOR_ENDERECAMENTO_MEMORIA);
      habilitarRegistradorBarramento();
      escreveBarramentos(REGISTRADOR_DADOS_MEMORIA);
      clockRegistradorBarramento();
      clockRegistradorDadosMemoria();
}

// Carrega dados para o operando A e depois para o acumulador
void carregamento() {
    aguardaPressionamentoBotaoPassoSimples();
    
    copiaRegistradorDadosMemoriaParaOperandoA();
    
    aguardaPressionamentoBotaoPassoSimples();
    
    ulaOperacaoA();
    habilitarRegistradorSaidaUla();
    clockRegistradorSaidaUla();
    clockRegistradorSaidaUla();
    
    aguardaPressionamentoBotaoPassoSimples();
    
    clockRegistradorAcumulador();
}

// Fluxo inicial de busca 
void fluxoBusca() {
   aguardaPressionamentoBotaoPassoSimples();
   
   copiaContadorProgramaParaRegistradorEnderecamentoMemoria();
  
   aguardaPressionamentoBotaoPassoSimples();
   
   incrementaContadorPrograma();
   
   leValorRegistradorEnderecoParaRegistradorDados();
}

// Copia dados do contator de programa para o registrador de enderecamento de memoria
void copiaContadorProgramaParaRegistradorEnderecamentoMemoria() {
  habilitarRegistradorContadorPrograma();
  clockRegistradorEnderecamentoMemoria();
  REGISTRADOR_ENDERECAMENTO_MEMORIA = CONTADOR_PROGRAMA;
}

// Copia dados do registrador de instrucoes para o registrador de dados da memoria
void copiaRegistradorInstrucoesParaRegistradorDadosMemoria() {
  REGISTRADOR_INSTRUCOES = REGISTRADOR_DADOS_MEMORIA;
  habilitarRegistradorDadosMemoria();
  clockRegistradorInstrucoes();
}

// Copia dados do registrador de enderecamento da memoria para o contator de programa
void copiaRegistradorEnderecamentoMemoriaParaContadorPrograma() {
  CONTADOR_PROGRAMA = REGISTRADOR_ENDERECAMENTO_MEMORIA;
  habilitarRegistradorEnderecamentoMemoria();
  clockRegistradorContadorPrograma();
}

// Copia dados do registrador de dados da memoria para o registrador de enderecamento da memoria
void copiaRegistradorDadosMemoriaParaRegistradorEnderecamentoMemoria() {
  REGISTRADOR_ENDERECAMENTO_MEMORIA = REGISTRADOR_DADOS_MEMORIA;
  habilitarRegistradorDadosMemoria();
  clockRegistradorEnderecamentoMemoria();
}

// Copia dados do acumulador para o registrador de dados da memoria
void copiaAcumuladorParaRegistradorDadosMemoria() {
  REGISTRADOR_DADOS_MEMORIA = ACUMULADOR;
  habilitarRegistradorAcumulador();
  clockRegistradorDadosMemoria(); 
}

// Copia dados do acumulador para o operando A
void copiaAcumuladorParaOperandoA() {
  OPERANDO_A = ACUMULADOR;
  habilitarRegistradorAcumulador();
  clockRegistradorA(); 
}

// Grava os dados resultantes da ULA no registrador de saida da ULA
void gravaDadosRegistradorUla() {
  habilitarRegistradorSaidaUla();
  clockRegistradorSaidaUla();
  clockRegistradorSaidaUla();
  clockRegistradorBarramentoRetorno();
  REGISTRADOR_SAIDA_ULA = leBarramentos();
}

// Copia dados da saida da ULA para o acumulador
void copiaSaidaUlaParaAcumulador() {
  habilitarRegistradorSaidaUla();
  clockRegistradorAcumulador();
  ACUMULADOR = REGISTRADOR_SAIDA_ULA;
}

// Copia dados do ponteiro da pilha para o registrador de enderecamento de memoria
void copiaPonteiroPilhaParaRegistradorEnderecamentoMemoria() {
  REGISTRADOR_ENDERECAMENTO_MEMORIA = PONTEIRO_PILHA;
  habilitarRegistradorPonteiroPilha();
  clockRegistradorEnderecamentoMemoria();
}

// Copia dados do registrador de dados da memoria para o operando A
void copiaRegistradorDadosMemoriaParaOperandoA() {
  OPERANDO_A = REGISTRADOR_DADOS_MEMORIA;
  habilitarRegistradorDadosMemoria();
  clockRegistradorA();
}

// Copia dados do registrador de dados da memoria para o operando B
void copiaRegistradorDadosMemoriaParaOperandoB() {
  habilitarRegistradorDadosMemoria();
  clockRegistradorB();
  OPERANDO_B = REGISTRADOR_DADOS_MEMORIA;
}

// Le dados do registrador de instrucoes e decodifica a instrucao
void leRegistradorInstrucoesDecodificando() {
  barramentosModoEntrada();
  habilitarRegistradorInstrucoes();
  clockRegistradorBarramentoRetorno();
  habilitarRegistradorBarramentoRetorno();
  DECODIFICADOR_INSTRUCOES = leBarramentos();
  decodificaInstrucao(DECODIFICADOR_INSTRUCOES);
}

// Busca a instrucao a ser executada
void buscaInstrucao() {
  fluxoBusca();
  
  aguardaPressionamentoBotaoPassoSimples();
  
  copiaRegistradorInstrucoesParaRegistradorDadosMemoria();
  
  aguardaPressionamentoBotaoPassoSimples();
  
  leRegistradorInstrucoesDecodificando();
}

// Decodificador de instrucoes
void decodificaInstrucao(byte instrucao) {
  switch (instrucao) {
    case HLT_IMEDIATO_UMA_PALAVRA: {
      limpaDadosLcd();
      lcd.print("DI:HLT");
    } break;
    case NOP_IMEDIATO_UMA_PALAVRA: {
      limpaDadosLcd();
      lcd.print("DI:NOP");
    } break;
    case NOT_IMEDIATO_UMA_PALAVRA: {
      limpaDadosLcd();
      lcd.print("DI:NOT");
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoNot();
      
      gravaDadosRegistradorUla();
      
      copiaSaidaUlaParaAcumulador();
    } break;
    case RET_IMEDIATO_UMA_PALAVRA: {
      limpaDadosLcd();
      lcd.print("DI:RET");
      
      aguardaPressionamentoBotaoPassoSimples();
      
      incrementaPonteiroPilha();
      
      copiaPonteiroPilhaParaRegistradorEnderecamentoMemoria();
      
      aguardaPressionamentoBotaoPassoSimples();
      
      leValorRegistradorEnderecoParaRegistradorDados();
      
      aguardaPressionamentoBotaoPassoSimples();
      
      //TODO
      
      buscaInstrucao();
      
      
    } break;
    case JMP_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:JMP[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      aguardaPressionamentoBotaoPassoSimples();
      
      //copiaRegistradorEnderecamentoMemoriaParaRegistradorDadosMemoria();
      
      aguardaPressionamentoBotaoPassoSimples();
      
      copiaRegistradorEnderecamentoMemoriaParaContadorPrograma();
      
      copiaRegistradorInstrucoesParaRegistradorDadosMemoria();
      
      aguardaPressionamentoBotaoPassoSimples();
  
      leRegistradorInstrucoesDecodificando();    
    } break;
    case JEQ_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case JGT_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case JGE_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case JCY_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case CAL_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case SHL_IMEDIATO_DUAS_PALAVRAS: {
      
    } break;
    case SHL_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case SHR_IMEDIATO_DUAS_PALAVRAS: {
      
    } break;
    case SHR_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case SRA_IMEDIATO_DUAS_PALAVRAS: {
      
    } break;
    case SRA_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case ROL_IMEDIATO_DUAS_PALAVRAS: {
      
    } break;
    case ROL_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case ROR_IMEDIATO_DUAS_PALAVRAS: {
      
    } break;
    case ROR_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case STO_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:STO[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      aguardaPressionamentoBotaoPassoSimples();
      
      copiaRegistradorDadosMemoriaParaRegistradorEnderecamentoMemoria();
      
      aguardaPressionamentoBotaoPassoSimples();
      
      copiaAcumuladorParaRegistradorDadosMemoria();
      
      aguardaPressionamentoBotaoPassoSimples();
      
      gravaEeprom(REGISTRADOR_ENDERECAMENTO_MEMORIA, REGISTRADOR_DADOS_MEMORIA);
      
      
    } break;
    case LOD_IMEDIATO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:LOD ");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      
      fluxoBusca();
      
      carregamento();
    } break;
    case LOD_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:LOD[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      leituraEnderecamentoDireto();

      carregamento();     
    } break;
    case CMP_IMEDIATO_DUAS_PALAVRAS: {
      
    } break;
    case CMP_DIRETO_DUAS_PALAVRAS: {
      
    } break;
    case ADD_IMEDIATO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:ADD ");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      
      fluxoBusca();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoAdd();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case ADD_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:ADD[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      leituraEnderecamentoDireto();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoAdd();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case SUB_IMEDIATO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:SUB ");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      
      fluxoBusca();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoSub();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case SUB_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:SUB[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      leituraEnderecamentoDireto();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoSub();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case AND_IMEDIATO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:AND ");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      
      fluxoBusca();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoAnd();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case AND_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:AND[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      leituraEnderecamentoDireto();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoAnd();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case XOR_IMEDIATO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:XOR ");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      
      fluxoBusca();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoXor();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case XOR_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:XOR[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      leituraEnderecamentoDireto();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoXor();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case ORL_IMEDIATO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:ORL ");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      
      fluxoBusca();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoOrl();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    case ORL_DIRETO_DUAS_PALAVRAS: {
      limpaDadosLcd();
      lcd.print("DI:ORL[");
      imprimeHexaLcd(lerEeprom(CONTADOR_PROGRAMA));
      lcd.print("]");
      
      fluxoBusca();
      
      leituraEnderecamentoDireto();
      
      copiaAcumuladorParaOperandoA();
      
      ulaOperacaoA();  
      
      copiaRegistradorDadosMemoriaParaOperandoB();
      
      ulaOperacaoOrl();
      
      gravaDadosRegistradorUla();
      copiaSaidaUlaParaAcumulador();
    } break;
    default: {
      opCodeInvalido();
      while(true) {
      }
    }
  }  
  if (instrucao != HLT_IMEDIATO_UMA_PALAVRA) {
    buscaInstrucao();
  }
}

// Imprime valor em base hexadecimal no display de LCD
void imprimeHexaLcd(byte valor) {
  if (valor < DEZESSEIS_HEXADECIMAL) {
    lcd.print(ZERO_HEXADECIMAL, HEX);
  }
  lcd.print(valor, HEX);
}

// Selecao de operacoes da ULA (Circuito Integrado: 74181)
void selecionaOperacaoUla(bool cn, bool modo, bool s3, bool s2, bool s1, bool s0) {
  digitalWrite(ULA_CN, cn);
  digitalWrite(ULA_MODO, modo);
  digitalWrite(ULA_SELECAO_3, s3);
  digitalWrite(ULA_SELECAO_2, s2);
  digitalWrite(ULA_SELECAO_1, s1);
  digitalWrite(ULA_SELECAO_0, s0);
}
void ulaOperacaoNot() {
  selecionaOperacaoUla(LOW, HIGH, LOW, LOW, LOW, LOW);
}
void ulaOperacaoAdd() {
  selecionaOperacaoUla(HIGH, LOW, HIGH, LOW, LOW, HIGH);
}
void ulaOperacaoSub() {
  selecionaOperacaoUla(LOW, LOW, LOW, HIGH, HIGH, LOW);
}
void ulaOperacaoAnd() {
  selecionaOperacaoUla(LOW, HIGH, HIGH, LOW, HIGH, HIGH);
}
void ulaOperacaoXor() {
  selecionaOperacaoUla(LOW, HIGH, LOW, HIGH, HIGH, LOW);
}
void ulaOperacaoOrl() {
  selecionaOperacaoUla(LOW, HIGH, HIGH, HIGH, HIGH, LOW);
}
void ulaOperacaoA() {
  selecionaOperacaoUla(HIGH, LOW, LOW, LOW, LOW, LOW);
}



// Indica instrução de uma/duas palavras (0/1)
bool quantidadePalavrasInstrucao(byte instrucao) {
   return (instrucao | (0 << BIT_7));
}

// Indica endereçamento imediato/direto (0/1)
bool tipoEnderecamentoInstrucao(byte instrucao) {
   return (instrucao | (0 << BIT_6));
}

// Incrementa o contator de programa
void incrementaContadorPrograma() {
  if (CONTADOR_PROGRAMA == ENDERECO_FINAL_CONTADOR_PROGRAMA) {
    CONTADOR_PROGRAMA = ENDERECO_INICIAL_CONTADOR_PROGRAMA;
  } else {
    CONTADOR_PROGRAMA += 0b1;
  }
  escreveBarramentos(CONTADOR_PROGRAMA);
  habilitarRegistradorBarramento();
  clockRegistradorBarramento();
  clockRegistradorContadorPrograma();
}

// Incrementa o ponteiro da pilha
void incrementaPonteiroPilha() {
  if (PONTEIRO_PILHA == ENDERECO_FINAL_CONTADOR_PROGRAMA) {
    PONTEIRO_PILHA = ENDERECO_INICIAL_CONTADOR_PROGRAMA;
  } else {
    PONTEIRO_PILHA += 0b1;
  }
  habilitarRegistradorBarramento();
  escreveBarramentos(PONTEIRO_PILHA);
  clockRegistradorBarramento();
  clockRegistradorPonteiroPilha();
}

// Decrementa o ponteiro da pilha
void decrementaPonteiroPilha() {
  if (PONTEIRO_PILHA == ENDERECO_INICIAL_CONTADOR_PROGRAMA)
    PONTEIRO_PILHA = ENDERECO_FINAL_CONTADOR_PROGRAMA;
  else 
    PONTEIRO_PILHA -= 0b1;
}

// Limpa dados do display de LCD
void limpaDadosLcd() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

// Coloca todos os registradores com os valores iniciais
void limpaDadosRegistradores() {
  escreveBarramentos(ZERO_BINARIO);
  habilitarRegistradorBarramento();
  clockRegistradorBarramento();  
  clockRegistradorA();
  clockRegistradorB();
  clockRegistradorSaidaUla();
  clockRegistradorAcumulador();
  clockRegistradorDadosMemoria();
  clockRegistradorEnderecamentoMemoria();
  clockRegistradorContadorPrograma();
  clockRegistradorInstrucoes();
  
  escreveBarramentos(ENDERECO_FINAL_CONTADOR_PROGRAMA);
  clockRegistradorBarramento();
  clockRegistradorPonteiroPilha();
}

// Coloca valor zero em todas as posições de memória especificadas
void limpaEeprom() {
  for (int i = POSICAO_INICIAL_EEPROM; i < POSICAO_FINAL_EEPROM; i++)
    EEPROM.write(i, VALOR_ZERO);
}

// Inicia as posições de memória com valores especificados abaixo
void iniciaEeprom() {
  EEPROM.write(0, 0xC4);
  EEPROM.write(1, 0x03);
  EEPROM.write(2, 0x00);
  EEPROM.write(3, 0x07);
  EEPROM.write(4, 0x00);
  EEPROM.write(5, 0x00);
  EEPROM.write(6, 0x00);
  EEPROM.write(7, 0x00);
  EEPROM.write(8, 0x00);
  EEPROM.write(9, 0x00);
}

// Grava dado na posicao de memoria passados por parametro
void gravaEeprom(byte endereco, byte dado) {
  EEPROM.write(endereco, dado);
}

// Le a posicao de memoria a partir do endereco passado por parametro
byte lerEeprom(byte endereco) {
    return EEPROM.read(endereco);
}

void setup() {
  // Define todas as 255 posicoes de memoria a serem usadas pela CPU-8E com o valor 00 (hexadecimal)
  limpaEeprom();
  
  // Carrega o programa a ser executado na CPU-8E de acordo com as posicoes e valores definidos na funcao escreveEeprom()
  iniciaEeprom();
  
  /* Encerra comunicação serial dos pinos 0, 1 e 14 ao 19 do Arduino MEGA, 
   sendo utilizados como portas digitais */
   //Serial.begin(9600);---------------------------------------------------
  Serial.end(); 
  
  Serial1.end();
  Serial2.end();
  Serial3.end();
  
  // Definicao dos modos (entrada ou saida) dos pinos do Arduino MEGA
  pinMode(HABILITAR_DEMUX_16, OUTPUT);
  pinMode(DEMUX_16_A, OUTPUT);
  pinMode(DEMUX_16_B, OUTPUT);
  pinMode(DEMUX_16_C, OUTPUT);
  pinMode(DEMUX_16_D, OUTPUT);
  pinMode(HABILITAR_DEMUX_16_1, OUTPUT);
  pinMode(DEMUX_16_1_A, OUTPUT);
  pinMode(DEMUX_16_1_B, OUTPUT);
  pinMode(DEMUX_16_1_C, OUTPUT);
  pinMode(DEMUX_16_1_D, OUTPUT);
  pinMode(ULA_CN, OUTPUT);
  pinMode(ULA_MODO, OUTPUT);
  pinMode(ULA_SELECAO_0, OUTPUT);
  pinMode(ULA_SELECAO_1, OUTPUT);
  pinMode(ULA_SELECAO_2, OUTPUT);
  pinMode(ULA_SELECAO_3, OUTPUT);
  pinMode(OPERACAO_ARDUINO, OUTPUT);
  pinMode(FLAG_ARDUINO_ZERO, OUTPUT);
  pinMode(FLAG_ARDUINO_NEGATIVO, OUTPUT);
  pinMode(FLAG_ARDUINO_CARRY, OUTPUT);
  pinMode(BOTAO_CICLO_SIMPLES, INPUT);
  pinMode(BOTAO_PASSO_SIMPLES, INPUT);
  
  // Definicao de parametros iniciais para o funcionamento da CPU-8E
  digitalWrite(OPERACAO_ARDUINO, HIGH);
  ulaOperacaoA();  
  digitalWrite(FLAG_ARDUINO_ZERO, LOW);
  digitalWrite(FLAG_ARDUINO_NEGATIVO, LOW);
  digitalWrite(FLAG_ARDUINO_CARRY, LOW);
  
  limpaDadosRegistradores();
  
  // Define a quantidade de colunas e linhas respectivamente do display de LCD
  lcd.begin(16, 2);
  limpaDadosLcd();
 
  lcd.print("DI:HLT");
  
  buscaInstrucao();
}

void loop() {
}
