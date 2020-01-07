DATA  SEGMENT
TABLE1 DB 4DH,45H,3DH,39H,33H,2DH,28H    ;1,2,3,4,5,6,7的频率
   DATA	ENDS 
STACK  SEGMENT STACK
        DW 64 DUP(?)   
STACK  ENDS
   CODE  SEGMENT
        ASSUME CS:CODE,DS:STACK
START: MOV AX, DATA
          MOV DS, AX
          MOV  AL,82H            ;8255初始化
          OUT  63H,AL
      A0: IN   AL,61H
          OUT  60H,AL
          CMP AL,00H        
          JZ  A0
          CMP AL,02H             ;1，2，3，4，5，6，7的发音判断
          JZ  F1
          CMP AL,04H        
          JZ  F2
          CMP AL,08H
          JZ  F3
          CMP AL,10H
          JZ  F4
          CMP AL,20H
          JZ  F5
          CMP AL,40H
          JZ  F6
          CMP AL,80H
          JZ  F7
          LOOP A0

     F1:  MOV BX, OFFSET TABLE1       ;发音1
          CALL FY
          CALL DELAY
          JMP   A0
     F2:  MOV BX, OFFSET TABLE1        ;发音2
          INC BX
          CALL FY
          CALL DELAY
          JMP   A0
     F3:  MOV BX, OFFSET TABLE1       ;发音3
          ADD BX,2
          CALL FY
          CALL DELAY
          JMP  A0
     F4:  MOV BX, OFFSET TABLE1         ;发音4
          ADD BX,3
          CALL FY
          CALL DELAY
          JMP  A0
     F5:  MOV BX, OFFSET TABLE1          ;发音5
          ADD BX,4
          CALL FY
          CALL DELAY
          JMP A0
     F6:  MOV BX, OFFSET TABLE1          ;发音6
          ADD BX,5
          CALL FY
          CALL DELAY
          JMP  A0
     F7:  MOV BX, OFFSET TABLE1          ;发音7
          ADD BX,6
          CALL FY
          CALL DELAY
          JMP   A0
      FY PROC                 ;发音子程序         
     A7:  MOV AL, 0B6H        ;8253初始化
          OUT 43H, AL
          MOV AL, [BX]
          MOV AH, 00H  
          MOV DL, 25H
       MUL DL    
          OUT 42H, AL                 
          MOV AL, AH
          OUT 42H, AL
          RET
      FY  ENDP
DELAY PROC                          ;延时子程序
          PUSH CX
          PUSH AX
          MOV CX,0040H
     A5:  MOV AX,056CH
     A6:  DEC AX
          JNZ A6
          LOOP A5
          POP AX
          POP CX
          MOV AL,0BAH      ;改变8253的控制字1011 1010,工作于方式5
          MOV DX,43H
          OUT DX,AL
          RET
DELAY ENDP
CODE	ENDS        
       END START