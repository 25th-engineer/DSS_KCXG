	.model	tiny	
addr_8255_pa	equ	0270h	
addr_8255_pb	equ	0271h	
addr_8255_c	equ	273h	
addr_273	equ	230h
line	equ	addr_273	
row1	equ	addr_8255_pa	
row2	equ	addr_8255_pb		
		.stack	100	
	.data		
wo db 00h,00h,04h,00h,0ch,07h,04h,01h,35h,86h,44h,58h,04h,60h,0ffh,90h,04h,08h,44h,84h,0c4h,40h,7fh,0feh,24h,21h,24h,12h,24h,10h,04h,00h
ai db 00h,00h,0ch,00h,0ah,02h,89h,03h,0a9h,42h,99h,64h,89h,54h,49h,48h,79h,54h,4fh,62h,49h,0c1h,59h,31h,69h,0ch,49h,02h,0dh,00h,02h,00h
wei db 00h,00h,08h,02h,1eh,03h,09h,0cch,0e8h,30h,1bh,0cch,04h,12h,3dh,09h,05h,7ch,0fdh,40h,05h,40h,3dh,7ch,0c5h,02h,23h,0ffh,11h,00h,08h,80h
ji db 00h,00h,00h,0eh,00h,02h,20h,02h,7fh,0fch,20h,00h,20h,00h,20h,00h,3fh,0f8h,00h,04h,08h,82h,09h,01h,0ffh,0ffh,0bh,00h,08h,0c0h,08h,20h
xin db 00h,00h,00h,00h,00h,00h,03h,00h,07h,0c0h,0ch,0e0h,0ch,30h,06h,18h,03h,0ch,03h,0ch,06h,18h,0ch,30h,0ch,0e0h,07h,0c0h,03h,00h,00h,00h

none   db 00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
	   db 00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
	.code		
start:	mov	ax,@data	
	mov	ds,ax	
	mov	es,ax	
	nop		
	call	init_io	
	call	test_led		
	call	clear	
chs_show:
	mov	cx,5	
	lea	si,wo	
chs_1:	push	cx	
	mov	cx,16		
chs_2:	call	disp_ch	
	inc	si	
	inc	si	
	loop	chs_2	
	pop	cx	
	loop	chs_1	
	jmp	chs_show	
disp_ch	proc	near	
	push	cx	
	mov	cx,20	
disp_ch_1:
	call	disp1	
	loop	disp_ch_1	
	pop	cx	
	ret		
disp_ch	endp	
	
disp1	proc	near	
	push	si	
	push	cx	
	mov	cx,16	
	
	mov	bl,01h	
	mov	bh,00h	
	
repeat:	mov	dx,row2
        mov 	al,bl
        out 	dx,al
        mov	dx,row1
        mov	al,bh
        out 	dx,al
        
        
        lodsb
   call adjust
        mov dl,al
        lodsb  
        call adjust
        mov ah,dl
        xchg 	al,ah
        xor 	ax,0ffffh
        mov 	dx,line
        out 	dx,ax	
	call	dl10ms
	call	clear	
	clc			
	rcl	bl,1	
	rcl	bh,1	
	loop	repeat	
	pop	cx	
	pop	si	
	ret		
disp1	endp		
init_io	proc	near	
	mov	dx,addr_8255_c	
	mov	al,80h	
	out	dx,al	;
	ret		
init_io		endp		
clear	proc	near	
	mov	al,0ffffh	
	mov	dx,line	
	out	dx,ax	
	mov	al,0	
	mov	dx,row1	
	out	dx,al	
	mov	dx,row2	
	out	dx,al
  
	ret		
clear	endp		

test_led	proc	near		
	mov	dx,line	
	xor	ax,ax	
	out	dx,ax		
	mov	al,0ffh	
	mov	dx,row1	
	out	dx,al	
	mov	dx,row2	
	out	dx,al	

	call	dl500ms	
	call	dl500ms	
	ret		
test_led	endp			

adjust 	proc		near	
	push		cx	
	mov	cx,8	
adjust1:	rcl	al,1	
	xchg	al,ah	
	rcr		al,1	
	xchg	al,ah	
	loop		adjust1	
	mov		al,ah	
	pop	cx	
	ret		
adjust 	endp	
		
dl10ms	proc	near	
	push	cx	
	mov	cx,133	
	loop	$	
	pop	cx	
	ret		
dl10ms	endp			
dl500ms	proc	near	
	push	cx	
	mov	cx,0ffffh	
	loop	$	
	pop	cx	
	ret		
dl500ms	endp					
	end	start