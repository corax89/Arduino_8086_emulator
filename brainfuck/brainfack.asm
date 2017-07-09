;FLAT asm
org     0x100    

start: 
;�������������� ����� ������ ������  
mov bx,offset array 
mov ax,offset array  
add ax,32
next0:
mov [bx],0  
inc bx
cmp bx,ax
jnz next0   
;�������� ����������� � �����
mov ah,02H   
mov dl,0Dh
int 21h  
mov dl,0Ah
int 21h
mov dl,'#'
int 21h
;��������� ������ � �����   
mov ah,0aH
mov dx,offset buffer 
int 21h 
;��������� ������
mov ah,02H   
mov dl,0Dh
int 21h  
mov dl,0Ah
int 21h  
;��������� ���������� ������
mov bx,offset buffer + 1  ;��������� �� ����� ��������� 
mov si,offset array       ;��������� �� ������ ������  
mov ch,0                  ;������� ������
char_compare:
inc bx    
;��������� �������
mov al,[bx]
cmp al,0;���� �� ����� ���� ��������� �� ������
jz start
;��������� �������
cmp al,'>'
jz next_cell
cmp al,'<'  
jz prev_cell
cmp al,'+'
jz inc_cell
cmp al,'-' 
jz dec_cell
cmp al,'.' 
jz print_cell
cmp al,'[' 
jz open_bracket
cmp al,']'  
jz loop_left
jmp char_compare

end: 
ret  
  
next_cell:  
;cmp bl,32
;jz char_compare
inc si
jmp char_compare    

prev_cell:  
;cmp bl,0
;jz char_compare
dec si
jmp char_compare  

inc_cell:  
;cmp bl,0
;jz char_compare 
inc [si]
jmp char_compare   

dec_cell:  
;cmp bl,0
;jz char_compare
dec [si]
jmp char_compare   

print_cell:  
;cmp bl,0
;jz char_compare
mov dl,[si]  
mov ah,02h
int 21h
jmp char_compare  

open_bracket:
cmp [si],0
;���� ������� ������ ����� 0 ������������� �� ����������� ������
jz loop_right           
jmp char_compare   

loop_right:
;��������� ��� ������� � ������� ����������� ������
cmp [bx],'['
jz add_bracket_r 
cmp [bx],']'
jz remove_bracket_r 
inc bx
cmp ch,0  
jnz loop_right  
;����������� �� ������ ����������� (����� ���������� ��� �����)
dec bx
dec bx
jmp char_compare  

add_bracket_r:
inc ch    
inc bx
jmp  loop_right 

remove_bracket_r:
dec ch 
inc bx
jmp  loop_right   

loop_left:
;��������� ��� ������� � ������� ����������� ������
cmp [bx],']'
jz add_bracket_l 
cmp [bx],'['
jz remove_bracket_l 
dec bx
cmp ch,0  
jnz loop_left  
;����������� �� ������� ����������
inc bx
jmp char_compare   

add_bracket_l:
inc ch    
dec bx
jmp  loop_left 

remove_bracket_l:
dec ch 
dec bx
jmp  loop_left 


;������ ��� ������� �����
array db ?, 32 dup (0), 0, 0         
;������ ��� ������
buffer db 255,?, 32 dup (0), 0, 0









