#ifndef _MG_CRT_H_
#define _MG_CRT_H_

#ifdef MG_CRT_MAIN

int _fltused;

void tinycrt_init();

int __stdcall 	main(void);
void __stdcall	mainCRTStartup(void);

void __stdcall
mainCRTStartup(void)
{
	main();
}
#endif // MG_CRT_MAIN

#endif // _MG_CRT_H_

