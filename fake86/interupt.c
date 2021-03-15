// This line added by @raspiduino
#include "cpu8086.h"
#include "cpu8086.c"

void videoBIOSinterupt(){
  switch(regs.byteregs[regah]){
  case 0x9:
  /*09H писать символ/атрибут в текущей позиции курсора
     вход:  BH = номер видео страницы
     AL = записываемый символ
     CX = счетчик (сколько экземпляров символа записать)
     BL = видео атрибут (текст) или цвет (графика)
      (графические режимы: +80H означает XOR с символом на экране)*/
  case 0xA:
    /*0aH писать символ в текущей позиции курсора
      вход:  BH = номер видео страницы
      AL = записываемый символ
      CX = счетчик (сколько экземпляров символа записать)*/
    for(uint16_t j=0;j<regs.wordregs[regcx];j++)
      Serial.print((char)(regs.byteregs[regal]));
    break;
  case 0xE:
  /*0eH писать символ на активную видео страницу (эмуляция телетайпа)
    вход:  AL = записываемый символ (использует существующий атрибут)
    BL = цвет переднего плана (для графических режимов)*/
    Serial.print((char)(regs.byteregs[regal]));
    break;
#ifdef DEBUG
    default:
      Serial.print("undefined videoBIOS interupt ");
      Serial.print(regs.byteregs[regah],HEX);
#endif
    }
}

void keyBIOSinterupt(){
  switch(regs.byteregs[regah]){
    case 0x0:
    /*00H читать (ожидать) следующую нажатую клавишу
    выход: AL = ASCII символ (если AL=0, AH содержит расширенный код ASCII )
          AH = сканкод  или расширенный код ASCII*/
    while(Serial.available() == 0){
      
    }
    regs.byteregs[regal] = (uint8_t) Serial.read();
    break;
#ifdef DEBUG
    default:
      Serial.print("undefined keyBIOS interupt ");
      Serial.print(regs.byteregs[regah],HEX);
#endif
  }
}

void DOSinterupt(){
  uint16_t adrs;
  switch(regs.byteregs[regah]){
    case 0x01:
      /*Вход AH = 01H
      Выход AL = символ, полученный из стандартного ввода
      Считывает (ожидает) символ со стандартного входного устройства. Отображает этот символ на стандартное выходное устройство (эхо)*/
      while(Serial.available() == 0){
        
      }
      regs.byteregs[regal] = (uint8_t) Serial.read();
      Serial.print((char)regs.byteregs[regal]);
      break; 
    case 0x02:
      Serial.print((char)regs.byteregs[regdl]);
      break;
    case 0x09:
      // AH=09h - вывод строки из DS:DX.
      for(uint8_t i = 0; i < 255; i++){
        char ch = (char)read86((segregs[regds] << 4) + regs.wordregs[regdx] + i);
        if(ch != '$')
          Serial.print(ch);
        else{
          regs.byteregs[regal] = 0x24;
          return;
        }
      }
      break;
    case 0x0a:
      /*DOS Fn 0aH: ввод строки в буфеp
      Вход AH = 0aH
      DS:DX = адрес входного буфера (смотри ниже)
      Выход нет = буфер содержит ввод, заканчивающийся символом CR (ASCII 0dH)*/
      adrs = (segregs[regds] << 4) + regs.wordregs[regdx];
      uint8_t length=0;
      char ch;
      ch=Serial.read();
      while(true){
        while(Serial.available() == 0){
          
        }
        ch=Serial.read();
        Serial.print(ch);
        if(ch=='\n' || ch=='\r')
          break;
        write86(adrs+length+2,(uint8_t) ch);
        length++;
        if(length>read86(adrs) || length>255)
          break;
      }
      write86(adrs+1,length);//записываем действительную длину данных
      write86(adrs+length+3,'$');
      break;
#ifdef DEBUG
    default:
      Serial.print("undefined DOS interupt ");
      Serial.print(regs.byteregs[regah],HEX);
#endif
    }
}
