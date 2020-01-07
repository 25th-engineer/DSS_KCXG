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
huan   db 00h,0c0h,00h,0c0h,0feh,0c0h,07h,0ffh,0c7h,86h,6fh,6ch,3ch,60h,18h,60h
db 1ch,60h,1ch,70h,36h,0f0h,36h,0d8h,61h,9ch,0c7h,0fh,3ch,06h,00h,00h
ying   db 60h,00h,31h,0c0h,3fh,7eh,36h,66h,06h,66h,06h,66h,0f6h,66h,36h,66h
db 37h,0e6h,37h,7eh,36h,6ch,30h,60h,30h,60h,78h,00h,0cfh,0ffh,00h,00h
shi	   db 00h,00h,06h,30h,07h,30h,0fh,0ffh,0ch,30h,1fh,0ffh,3bh,33h,7bh,33h
db 1bh,0ffh,1bh,33h,19h,0b0h,18h,0e0h,18h,60h,18h,0fch,19h,8fh,1fh,03h
yong   db 00,0,1fh,0feh,18h,0c6h,18h,0c6h,18h,0c6h,1fh,0feh,018h,0c6h,18h,0c6h
db 18h,0c6h,1fh,0feh,18h,0c6h,18h,0c6h,30h,0c6h,30h,0c6h,60h,0deh,0c0h,0cch
xing   db 00h,00h,1fh,0fch,18h,0ch,1fh,0fch,18h,0ch,1fh,0fch,01h,80h,19h,80h
db 1fh,0feh,31h,80h,31h,80h,6fh,0fch,01h,80h,01h,80h,7fh,0ffh,00h,00h
yan    db 0,0,0ffh,0ffh,18h,0cch,18h,0cch,30h,0cch,30h,0cch,7fh,0ffh,7ch,0cch
	   db 0fch,0cch,3ch,0cch,3ch,0cch,3dh,8ch,3dh,8ch,33h,0ch,06h,0ch,0ch,0ch
shi0   db 01h,80h,00h,0c0h,3fh,0ffh,3ch,06h,67h,0cch,06h,0c0h,0ch,0c0h,07h,0c0h
db 06h,0c0h,7fh,0ffh,00h,0c0h,01h,0e0h,03h,30h,06h,18h,1ch,1ch,70h,18h
yan0   db 00h,00h,0fch,60h,0ch,60h,6ch,0f0h,6ch,0d8h,6dh,8fh,6fh,0f8h,7eh,00h
db 06h,0c6h,07h,66h,3fh,0ech,0e7h,0ech,06h,18h,1fh,0ffh,0ch,00h,00h,00h
yi	   db 0ch,0c0h,0ch,60h,18h,7ch,1bh,6ch,33h,0ch,73h,18h,0f1h,98h,31h,98h
	   db 30h,0f0h,30h,0f0h,30h,60h,30h,0f0h,31h,98h,33h,0fh,3eh,06h,30h,00h
none   db 00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
	   db 00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h
	.code		
	start:	mov		ax,@data	
	mov	ds,ax	
	mov	es,ax	
	nop		
	call	init_io	
	call		test_led		
	call		clear	
chs_show:	mov		cx,9	
	lea		si,huan	
chs_1:	push	cx	
	mov	cx,16		
chs_2:	call	disp_ch	
	inc	si	
	inc	si	
	loop		chs_2	
	pop	cx	
	loop		chs_1	
	jmp	chs_show	
disp_ch	proc	near	
	push		cx	
	mov	cx,8	
disp_ch_1:	call	disp1	
	loop		disp_ch_1	
	pop		cx	
	ret		
disp_ch	endp		
disp1	proc	near	
	push		si	
	push		cx	
	mov	cx,16	
	mov		bl,0feh		
	mov	bh,0ffh	
repeat:		mov		dx,line	
	mov		ax,bx	
	out	dx,ax	
	lodsb		
	
	mov	dx,row1	
		out	dx,al	
	lodsb		

	mov	dx,row2	
	out		dx,al	
	call	dl10ms	
	call		clear	
	stc		
	rcl	bl,1	
	rcl	bh,1		
	loop	repeat	
	pop	cx	
	pop	si	
	ret		
disp1	endp		
init_io		proc	near	
	mov	dx,addr_8255_c	
	mov	al,80h	
	out	dx,al
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
	call		dl500ms	
	call		dl500ms	
	ret		
test_led		endp			

dl10ms	proc		near	
	push		cx	
	mov	cx,133	
	loop	$	
	pop		cx	
	ret		
dl10ms	endp			
dl500ms		proc	near	
	push		cx	
	mov		cx,0ffffh	
	loop		$	
	pop	cx	
	ret		
dl500ms	endp		
			
	end	start