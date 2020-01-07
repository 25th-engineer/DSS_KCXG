		.model tiny
c8255	equ	0273h
pa8255	equ 0270h
pc8255 	equ 0272h
		.stack 100
		.data
music		dw  m1,q3,m2,q3,m3,q3,m1,q3,m1,q3,m2,q3,m3,q3,m1,q3
                dw  m3,q2,m4,q2,m5,q3,m3,q2,m4,q2,m5,q3
                dw  m5,q2,m6,q2,m5,q2,m4,q2,m3,q2,m1,q2,m5,q2,m6,q2,m5,q2
                dw  m4,q2,m3,q2,m1,q2,m1,q2,m5,q2,m1,q2,m1,q2,m5,q2,m1				                                                                                                                                                                                                                                                                                                                
		.code
start: 
		call init8255			;8255 初始化
		call demo 				;播放一段音乐
start1: 
		mov dx, pa8255 			;按键查询
		in al, dx 				;读键值
		cmp al, 0ffh			;judge if al equals 11111111b
		jz start1 				;无键
		xor al, 0ffh 			;有键
		test al, 1
		jz start2
		call music1 			;1 号键 , 调 1 号键输出
		jmp start1
		
start2: 
		test al, 2				;2h = 00000010b
		jz start3
		call music2 			;2 号键
		jmp start1
start3: 
		test al, 4				;4h = 00000100b
		jz start4
		call music3 			;3 号键
		jmp start1
start4: 
		test al, 8				;8h = 00001000b
		jz start5
		call music4 			;4 号键
		jmp start1
start5: 
		test al, 10h			;10h = 00010000b
		jz start6
		call music5 			;5 号键
		jmp start1
start6: 
		test al, 20h			;20h = 00100000b
		jz start7
		call music6 			;6 号键
		jmp start1
start7: 
		test al, 40h			;40h = 01000000b
		jz start1
		call music7 			;7 号键
		jmp start1
		
demo	proc near
		mov cx, 80				;生日歌共 25 拍
	
		lea bx, music
		
		
	
demo10: 
		push cx					;保护 cx 的值
		
		call [bx] 				;播放 bx 指向的音阶
		
		;call [bx+50]
		;call [si]
		
		
		;call [bx]
		inc bx
		inc bx					;bx <- bx + 2，指向预定的下一个音阶
		;inc si
		;inc si
		
		
		pop cx					;恢复 cx 的值 
		loop demo10				;继续播放
		ret
demo	endp

								
music1	proc near				;节拍 1(手动按键时用 )
		call w_l 				;写 0, 蜂鸣器响
		call t10 				;延时 100us
		call t5 				;延时 50us
		call t5 				;延时 50us
		call w_h 				;写 1, 蜂鸣器不响
		call t10 				;延时
		call t5 				;
		call t5 				;
		ret
music1 	endp

								
music2 	proc near				;节拍 2, 同上
		call w_l 				;写 0, 蜂鸣器响
		call t10
		call t5
		call t2
		call t1
		call w_h 				;写 1, 蜂鸣器不响
		call t10
		call t5
		call t2
		call t2
		ret
music2 	endp

								
music3 	proc near				;节拍 3, 同上
		call w_l
		call t10
		call t5
		call t2
		call t1
		call w_h
		call t10
		call t5
		call t2
		ret
music3 	endp

								
music4 	proc near				;节拍 4, 同上
		call w_l
		call t10
		call t5
		call t1
		call w_h
		call t10
		call t5
		call t1
		ret
music4 	endp

music5 	proc near				;节拍 5, 同上
		call w_l
		call t10
		call t5
		call w_h
		call t10
		call t5
		ret
music5 	endp


music6 	proc near				;节拍 6, 同上
		call w_l
		call t10
		call t2
		call t2
		call w_h
		call t10
		call t2
		call t2
		ret
music6 	endp


music7 	proc near				;节拍 7, 同上
		call w_l
		call t10
		call t2
		call t1
		call w_h
		call t10
		call t2
		call t1
		ret
music7 	endp


;m1~m7每个节拍放音的时间基本相同，约为0.2s，通过修改函数体所call的函数体来改变每秒蜂鸣器震
;						荡的次数，以此来体现放音时不同的音阶。

m1 		proc near				;节拍 1(自动放音时用 , 时间约 0.2s)
		mov cx, 1100				;cx 控制的是下面两条loop指令执行的次
		 					;数，1100 共执行loop指令 550 次
m10: 
		call w_l
		call t10
		call t10
		call t2
		loop m11
m11: 
		call w_h
		call t10
		call t10
		call t1
		loop m10
		ret
m1 		endp


m2 		proc near				;节拍 2, 同上
		mov cx, 1150
m20: 
		call w_l
		call t10
		call t5
		call t2
		call t2
		loop m21
m21: 
		call w_h
		call t10
		call t5
		call t2
		call t2
		loop m20
		ret
m2 		endp

;节拍 3, 同上
m3 		proc near
		mov cx, 1200
m30: 
		call w_l
		call t10
		call t5
		call t2
		call t1
		push ax
		pop ax
		nop
		nop
		loop m31
m31: 
		call w_h
		call t10
		call t5
		call t2
		call t1
		loop m30
		ret
m3 		endp

;节拍 4, 同上
m4 		proc near
		mov cx, 1250
m40: 
		call w_l
		call t10
		call t5
		call t2
		call t1
		loop m41
m41: 
		call w_h
		call t10
		call t5
		call t2
		call t1
		loop m40
		ret
m4 		endp

;节拍 5, 同上
m5 		proc near
		mov cx, 1300
m50: 
		call w_l
		call t10
		call t5
		call t2
		loop m51
m51: 
		call w_h
		call t10
		call t5
		call t2
		loop m50
		ret
m5 		endp

;节拍 6, 同上
m6 		proc near
		mov cx, 1350
m60: 
		call w_l
		call t10
		call t5
		call t1
		loop m61
m61: 
		call w_h
		call t10
		call t5
		call t1
		loop m60
		ret
m6 		endp

;节拍 7, 同上
m7 		proc near
		mov cx, 1420
m70: 
		call w_l
		call t10
		call t5
		loop m71
m71: 
		call w_h
		call t10
		call t5
		loop m70
		ret
m7 		endp

					;写 0(8255.pc.7=0)，蜂鸣器响
w_l 	proc near
		mov dx, c8255
		mov al, 0eh
		out dx, al
		ret
w_l 	endp

;写 1(8255.pc.7=1)
w_h 	proc near
		mov dx, c8255
		mov al, 0fh
		out dx, al
		ret
w_h 	endp

;8255 初始化
init8255 proc near
		mov dx, c8255
		mov al, 90h ;pc.7输出 , pa输入
		out dx, al
		mov dx, c8255
		mov al, 0fh
		out dx, al
		ret
		init8255 endp
		
;延时 10us
t1		proc near
		ret
t1 		endp

;延时 20us
t2 		proc near
		call t1
		ret
t2 		endp

;延时 50us
t5 		proc near
		call t2
		call t2
		ret
t5 		endp

;延时 100us
t10 	proc near
		call t2
		call t2
		call t5
		ret
t10 	endp

q1      proc near
        mov cx,300
loopq1: call t10
        loop loopq1
        ret
q1      endp

q2      proc near
        call q1
        call q1
        ret
q2      endp

q3      proc near
        call q2
        call q2
        ret
q3      endp
		end start