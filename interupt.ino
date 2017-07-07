void videoBIOSinterupt(){
  switch(regs.byteregs[regah]){
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
#ifdef DEBUG
    default:
      Serial.print("undefined DOS interupt ");
      Serial.print(regs.byteregs[regah],HEX);
#endif
    }
}
