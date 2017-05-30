#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cwchar>

using namespace std;
//It is not my code -- start
int Deconvert_single_unicode_symbols(string &p)
{//deconvert unicode symbols like "\u0027"

	int return_val = 0;
	int lengh = p.size();
	if (!lengh)
		_asm nop;



	string temp_str(p.size()+20,'\0');

	int	len = 0;
	char stringg[8];
	char string2[8];

	int number;
	int order;
	int len2;
	int l;

	for (int f = 0;f<lengh;f++)

		if ((p[f] == 0x5c || p[f] == '%') && p[f + 1] == 'u')
		{
			if (p[f + 1] == 'u'
				&& (p[f + 2] >= '0'&&p[f + 2] <= '9' || p[f + 2] >= 'A'&&p[f + 2] <= 'F' || p[f + 2] >= 'a'&&p[f + 2] <= 'f')
				&& (p[f + 3] >= '0'&&p[f + 3] <= '9' || p[f + 3] >= 'A'&&p[f + 3] <= 'F' || p[f + 3] >= 'a'&&p[f + 3] <= 'f')
				&& (p[f + 4] >= '0'&&p[f + 4] <= '9' || p[f + 4] >= 'A'&&p[f + 4] <= 'F' || p[f + 4] >= 'a'&&p[f + 4] <= 'f')
				&& (p[f + 5] >= '0'&&p[f + 5] <= '9' || p[f + 5] >= 'A'&&p[f + 5] <= 'F' || p[f + 5] >= 'a'&&p[f + 5] <= 'f')
				)
			{
				return_val = 1;//mark not-english strings

				memset(stringg, 0, 8);
				memset(string2, 0, 8);
				for (int g = 0;g<4;g++)
					stringg[g] = (char)p[f + 2 + g];
				number = 0;
				order = 1;
				len2 = strlen(stringg) - 1;

				//if(len2<8)
				for (l = len2;l >= 0;l--)
				{
					if ((len2 - l == 1))order = 16;
					if ((len2 - l == 2))order = 16 * 16;
					if ((len2 - l == 3))order = 16 * 16 * 16;
					switch (stringg[l])
					{
					case 0x30:
					case 0x31:
					case 0x32:
					case 0x33:
					case 0x34:
					case 0x35:
					case 0x36:
					case 0x37:
					case 0x38:
					case 0x39:
						number += ((char)(stringg[l]) - 0x30)*order;
						break;
					case 'A':
					case 'a':
						number += 10 * order;
						break;
					case 'B':
					case 'b':
						number += 11 * order;
						break;
					case 'C':
					case 'c':
						number += 12 * order;
						break;
					case 'D':
					case 'd':
						number += 13 * order;
						break;
					case 'E':
					case 'e':
						number += 14 * order;
						break;
					case 'F':
					case 'f':
						number += 15 * order;
						break;
					default: break;
					}
				}

				if (number>255)
				{
					string2[0] = number & 255;
					string2[1] = (number >> 8) & 255;
				}
				else
				{
					string2[1] = 0;
					string2[0] = number & 255;
				}

				memset(stringg, 0, 8);
				WideCharToMultiByte(CP_UTF8, 0, (WCHAR*)string2, wcslen((WCHAR*)string2), stringg, 8, NULL, NULL);

				memcpy(&temp_str[len], stringg, strlen(stringg));
				len += strlen(stringg);
				f += 5;

				continue;
			}


		}
		else
			if (p[f + 0] == '&'&& p[f + 1] == '#' && p[f + 2] >= '0'&&p[f + 2] <= '9'&&p[f + 3] >= '0'&&p[f + 3] <= '9'&& p[f + 4] >= '0'&&p[f + 4] <= '9'&&p[f + 5] == ';')
			{
				memset(stringg, 0, 8);
				for (int g = 0;g<6;g++)
					if (p[f + 2 + g] != (char)';')
						stringg[g] = (char)p[f + 2 + g];
					else
						break;
				number = (int)atoi(stringg);
				//-----------------
				if (number >= (unsigned char)0x80)
				{
					temp_str[len] = (char)0xc2;len++;
				}
				//------------------------
				temp_str[len] = (char)number;//&#183;

				len++;
				f += 5;
				continue;
			}
			else
				if (p[f + 0] == '%' && p[f + 1] >= '0'&&p[f + 1] <= 'F'&&p[f + 2] >= '0'&&p[f + 2] <= 'F')
				{

					memset(stringg, 0, 8);
					for (int g = 0;g<2;g++)
						stringg[g] = (char)p[f + 1 + g];


					number = strtol(stringg, NULL, 16);
					//	number=(p[f+1]-0x30)*16+(p[f+2]-0x30);

					//------------------------
					temp_str[len] = (char)number;//%20;

					len++;
					f += 2;
					continue;
				}

				else
					if (p[f + 0] == '&'&& p[f + 1] == '#' && p[f + 2] >= '0'&&p[f + 2] <= '9'&&p[f + 3] >= '0'&&p[f + 3] <= '9'&& p[f + 4] >= '0'&&p[f + 4] <= '9'&& p[f + 5] >= '0'&&p[f + 5] <= '9'&&p[f + 6] == ';')
					{
						memset(stringg, 0, 8);
						memset(string2, 0, 8);
						for (int g = 0;g<6;g++)
							if (p[f + 2 + g] != (char)';')
								stringg[g] = (char)p[f + 2 + g];
							else
								break;
						number = (int)atoi(stringg);
						//-----------------
						/*	if(number>=(unsigned char)0x80)
						{	temp_str[len]=(char)0xc2;len++;}
						//------------------------

						memcpy(&temp_str[len],string,strlen(string));

						temp_str[len]=(char)number;//&#183;*/
						if (number>255)
						{
							string2[0] = number & 255;
							string2[1] = (number >> 8) & 255;
						}
						else
						{
							string2[1] = 0;
							string2[0] = number & 255;
						}

						memset(stringg, 0, 8);
						WideCharToMultiByte(CP_UTF8, 0, (WCHAR*)string2, wcslen((WCHAR*)string2), stringg, 8, NULL, NULL);

						memcpy(&temp_str[len], stringg, strlen(stringg));

						len += strlen(stringg);
						f += 6;
						continue;
					}

					else
					{
						if ((unsigned char)p[f] >= (unsigned char)0x80)
							return_val = 1;//mark not-english strings

						temp_str[len] = (char)(p[f]);
						len++;
					}

	temp_str[len] = 0;
	p = temp_str;

	return(return_val);

}


void DeConvert_str(string &p)
{//convert string from xml
	int lengh = p.size();
	if (!lengh)
		_asm nop;

	int	len = 0;
	int f, number = 0;
	//char string[32];
	
	string szSTR3(p.size()+20,'\0');

	//for( f=0;f<lengh;f++)
	//		if(p[f]==(unsigned char)'%')
	//		f=f;


	for (f = 0;f<lengh;f++)
		if (p[f] == '&')
		{
			if (p[f + 1] == 'a'&&p[f + 2] == 'm'&&p[f + 3] == 'p'&&p[f + 4] == ';')
			{
				szSTR3[len] = (char)'&';//&amp; («&»)
				len++;
				f += 4;
				continue;
			}
			if (p[f + 1] == 'l'&&p[f + 2] == 't'&&p[f + 3] == ';') //&lt;
			{
				//if(p[f+4]=='b')
				//	f=f;

				szSTR3[len] = (char)'<';
				len++;
				f += 3;
				continue;
			}
			if (p[f + 1] == 'g'&&p[f + 2] == 't'&&p[f + 3] == ';')//&gt;
			{
				szSTR3[len] = (char)'>';
				len++;
				f += 3;
				continue;
			}
			if (p[f + 1] == 'a'&&p[f + 2] == 'p'&&p[f + 3] == 'o'&&p[f + 4] == 's'&&p[f + 5] == ';')
			{
				szSTR3[len] = (char)0x27;//&apos; («'»),
				len++;
				f += 5;
				continue;
			}
			if (p[f + 1] == 'r'&&p[f + 2] == 'd'&&p[f + 3] == 'q'&&p[f + 4] == 'u'&&p[f + 5] == 'o'&&p[f + 6] == ';')//&rdquo;
			{
				szSTR3[len] = (char)'"';
				len++;
				f += 6;
				continue;
			}

			if (p[f + 1] == 'l'&&p[f + 2] == 'd'&&p[f + 3] == 'q'&&p[f + 4] == 'u'&&p[f + 5] == 'o'&&p[f + 6] == ';')//&ldquo;
			{
				szSTR3[len] = (char)'"';
				len++;
				f += 6;
				continue;
			}

			if (p[f + 1] == 'n'&&p[f + 2] == 'b'&&p[f + 3] == 's'&&p[f + 4] == 'p'&&p[f + 5] == ';')
			{
				szSTR3[len] = (char)(p[f]);
				len++;
				szSTR3[len] = (char)(p[f + 1]);
				len++;
				szSTR3[len] = (char)(p[f + 2]);
				len++;
				szSTR3[len] = (char)(p[f + 3]);
				len++;
				szSTR3[len] = (char)(p[f + 4]);
				len++;
				szSTR3[len] = (char)(p[f + 5]);
				len++;

				f += 5;
				continue;
			}

			if (p[f + 1] == 'c'&&p[f + 2] == 'o'&&p[f + 3] == 'p'&&p[f + 4] == 'y'&&p[f + 5] == ';')//&copy; ©
			{
				szSTR3[len] = (char)0xC2;//&copy;
				len++;
				szSTR3[len] = (char)0xA9;//&copy;
				len++;

				f += 5;
				continue;
			}




			if (p[f + 1] == 'q'&&p[f + 2] == 'u'&&p[f + 3] == 'o'&&p[f + 4] == 't'&&p[f + 5] == ';')
			{
				szSTR3[len] = (char)'"';	// &quot; (""")
				len++;
				f += 5;
				continue;
			}

			if (p[f + 1] == ' ')
			{
				szSTR3[len] = (char)(p[f]);
				len++;
				f++;
				continue;
			}

			szSTR3[len] = (char)(p[f]);//just "&"
			len++;

		}
		else
		{
			szSTR3[len] = (char)(p[f]);
			len++;
		}

	szSTR3[len] = 0;
	p = szSTR3;
	lengh = len;
}


void DeConvert_strGoogle(string &p)
{
	int	len = 0;
	int f, number = 0;
	char stringg[32];

	string szSTR3(p.size()+15,'\0');
	int lengh = p.size();

	for (f = 0;f<lengh;f++)
		if (p[f] == '&')//&#39;
		{
			if (p[f + 1] == '#'&&p[f + 2] >= '0'&&p[f + 2] <= '9'&&p[f + 3] >= '0'&&p[f + 3] <= '9'&&p[f + 4] == ';') //&#XX;
			{
				memset(stringg, 0, 32);
				for (int g = 0;g<6;g++)
					if (p[f + 2 + g] != (char)';')
						stringg[g] = (char)p[f + 2 + g];
					else
						break;
				number = (int)atoi(stringg);
				szSTR3[len] = (char)number;//&#39;

				len++;
				f += 4;
				continue;
			}
			else
				if (p[f + 1] == '#'&&p[f + 2] == 'x' &&p[f + 3] == 'D'&&p[f + 4] == ';') //&#xD;
				{
					szSTR3[len] = (char)0x0D;//&#39;

					len++;
					f += 4;
				}
				else

				{
					szSTR3[len] = (char)(p[f]);
					len++;
				}

		}
		else
			if (p[f + 0] == '#')
			{
				if (p[f + 0] == '#'&&p[f + 1] >= '0'&&p[f + 1] <= '9'&&p[f + 2] >= '0'&&p[f + 2] <= '9'&&p[f + 3] == ';')//#XX;
				{
					memset(stringg, 0, 32);
					for (int g = 0;g<6;g++)
						if (p[f + 1 + g] != (char)';')
							stringg[g] = (char)p[f + 1 + g];
						else
							break;
					number = (int)atoi(stringg);
					szSTR3[len] = (char)number;//#39;

					len++;
					f += 3;
					continue;
				}
				else
					if (p[f + 0] == '#'&&p[f + 1] == 'x' &&p[f + 2] == 'D'&&p[f + 3] == ';')//#xD;
					{
						szSTR3[len] = 0x0D;
						//szSTR3[len+1]=0x0A;
						len++;//len++;
						f += 3;
						continue;

					}
					else
					{
						szSTR3[len] = (char)(p[f]);
						len++;
					}
			}
			else
				if (p[f + 0] == '\\')
				{		//========================================
					if (p[f + 0] == '\\'&&p[f + 1] == 'n')	// \n
					{
						szSTR3[len] = 0x0A;
						len++;
						f += 1;
						continue;
					}
					else
						if (p[f + 0] == '\\'&&p[f + 1] == 'r')	// \r
						{
							szSTR3[len] = 0xD;
							len++;
							f += 1;
							continue;
						}//======================================
						else
							if (p[f + 0] == (char)'\\'&&p[f + 1] == (char)' '&&p[f + 2] == (char)0xd0 && p[f + 3] == (char)0x9f
								&& p[f + 4] == (char)' '&&p[f + 5] == (char)'\\'&&p[f + 6] == (char)' '&&p[f + 7] == (char)'n')	//\ Ï \ n //rus
							{
								szSTR3[len] = '\\';
								szSTR3[len + 1] = 'n';
								szSTR3[len + 2] = '\\';
								szSTR3[len + 3] = 'n';
								len++;len++;
								len++;len++;
								f += 7;;
								continue;
							}
							else
								if (p[f + 0] == (char)'\\'&&p[f + 1] == (char)' '&&p[f + 2] == (char)'N'&&p[f + 3] == (char)' '
									&&p[f + 4] == (char)'\\'&&p[f + 5] == (char)' '&&p[f + 6] == (char)'n')	//\ N \ n
								{
									szSTR3[len] = '\\';
									szSTR3[len + 1] = 'n';
									szSTR3[len + 2] = '\\';
									szSTR3[len + 3] = 'n';
									len++;len++;
									len++;len++;
									f += 3;	f += 3;
									continue;
								}
								else
									if (p[f + 0] == (char)'\\'&&p[f + 1] == (char)' '&&p[f + 2] == (char)0xd0 && p[f + 3] == (char)0x9f)	// \ Ï //rus
									{
										szSTR3[len] = '\\';
										szSTR3[len + 1] = 'n';
										len++;len++;
										f += 3;
										continue;
									}
									else
										if (p[f + 0] == (char)'\\'&&p[f + 1] == ' '&&p[f + 2] == (char)'N')	// \ N 
										{
											szSTR3[len] = '\\';
											szSTR3[len + 1] = 'n';
											len++;len++;
											f += 2;
											continue;
										}
										else
											if (p[f + 0] == (char)'\\'&&p[f + 1] == (char)' '&&p[f + 2] == (char)0xd0 && p[f + 3] == (char)0xbf)	// \ n
											{
												szSTR3[len] = '\\';
												szSTR3[len + 1] = 'n';
												len++;len++;
												f += 3;
												continue;
											}
											else
												if (p[f + 0] == '\\'&&p[f + 1] == ' '&&p[f + 2] == 'n')	// \ n
												{
													szSTR3[len] = '\\';
													szSTR3[len + 1] = 'n';
													len++;len++;
													f += 2;
													continue;
												}
												else
												{
													if (p[f + 0] == '\\'&&p[f + 1] == ' '&&p[f + 2] == '"')	// \ "
													{
														szSTR3[len] = '\\';
														szSTR3[len + 1] = '"';
														len++;len++;
														f += 2;
														continue;
													}
													else
													szSTR3[len] = (char)(p[f]);
													len++;
												}

				}
				else
					if (p[f + 0] == '%')
					{
						if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)'D')	// % D
						{
							if (f>0) { szSTR3[len] = ' ';len++; }
							szSTR3[len] = '%';len++;
							szSTR3[len] = 'd';len++;

							f += 2;
							continue;
						}
						else
							if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)'d')	// % D
							{
								if (f>0) { szSTR3[len] = ' ';len++; }
								szSTR3[len] = '%';len++;
								szSTR3[len] = 'd';len++;

								f += 2;
								continue;
							}
							else
							if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)0xd0 && (p[f + 3] == (char)0x93 || p[f + 3] == (char)0xB3))	// % Ã % ã//rus
							{
								if (f>0) { szSTR3[len] = ' ';len++; }
								szSTR3[len] = '%';len++;
								szSTR3[len] = 'd';len++;

								f += 3;
								continue;
							}
							else
								if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)0xd1 && p[f + 3] == (char)0x81)	// % c //rus
								{
									if (f>0) { szSTR3[len] = ' ';len++; }
									szSTR3[len] = '%';len++;
									szSTR3[len] = 's';len++;

									f += 3;
									continue;
								}
								else
									if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)'S'&&p[f + 3] == (char)'.')	// % S. //rus
									{
										if (f>0) { szSTR3[len] = ' ';len++; }
										szSTR3[len] = '%';len++;
										szSTR3[len] = 's';len++;

										f += 3;
										continue;
									}
									else
										if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)'s'&&p[f + 3] == (char)'.')	// % s. //rus
										{
											if (f>0) { szSTR3[len] = ' ';len++; }
											szSTR3[len] = '%';len++;
											szSTR3[len] = 's';len++;

											f += 3;
											continue;
										}
										else
											if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)'s')	// % s  
											{
												if (f>0) { szSTR3[len] = ' ';len++; }
												szSTR3[len] = '%';len++;
												szSTR3[len] = 's';len++;

												f += 2;
												continue;
											}
											else
												if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' '&&p[f + 2] == (char)'S')	// % S  
												{
													if (f>0) { szSTR3[len] = ' ';len++; }
													szSTR3[len] = '%';len++;
													szSTR3[len] = 's';len++;

													f += 2;
													continue;
												}
												else
													if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' ' && (p[f + 2] >= (char)'0'&&p[f + 2] <= (char)'9') && p[f + 3] == (char)' '
														&&p[f + 4] == (char)'$'&&p[f + 5] == (char)' ' && (p[f + 6] == (char)'S' || p[f + 6] == (char)'s'))				// 	% 1 $ S
													{//DNS1: 5% ñ $
														if (f>0) { szSTR3[len] = ' ';len++; }
														szSTR3[len] = '%';len++;
														szSTR3[len] = p[f + 2];len++;
														szSTR3[len] = '$';len++;
														szSTR3[len] = 's';len++;

														f += 6;
														continue;
													}
													else
														if (p[f + 0] == (char)'%'&&p[f + 1] == (char)' ' && (p[f + 2] >= (char)'0'&&p[f + 2] <= (char)'9') && p[f + 3] == (char)' '
															&&p[f + 4] == (char)'$'&&p[f + 5] == (char)' '&& p[f + 6] == (char)0xD1 && p[f + 7] == (char)0x81)//DNS1: 5% ñ $				// 	% 1 $ S
														{
															if (f>0) { szSTR3[len] = ' ';len++; }
															szSTR3[len] = '%';len++;
															szSTR3[len] = p[f + 2];len++;
															szSTR3[len] = '$';len++;
															szSTR3[len] = 's';len++;
															f += 7;
															continue;
														}
														else
														{
															szSTR3[len] = (char)(p[f]);
															len++;
														}

					}
					else
						if (p[f + 0] == '<')
						{
							if (p[f + 0] == (char)'<'&&p[f + 1] == (char)'b'&&p[f + 2] == (char)'r'&&p[f + 3] == (char)'>') //gogle
							{
								szSTR3[len] = 0x0A;//	szSTR3[len]=0x0d
												   //szSTR3[len+1]=0x0A;
								len++;//len++;
								f += 3;
								continue;
							}
							else
								if (p[f + 0] == (char)'<'&&p[f + 1] == (char)'b'&&p[f + 2] == (char)'r'&&p[f + 3] == (char)'/'&&p[f + 4] == (char)'>') //promt
								{
									//szSTR3[len]=0x0A;
									szSTR3[len] = 0x0d;
									szSTR3[len + 1] = 0x0A;
									len++;len++;
									f += 4;
									continue;
								}
								else
									if (p[f + 0] == (char)'<'&&p[f + 1] == (char)'//'&&p[f + 2] == (char)' '&&p[f + 3] == (char)'X'
										&&p[f + 4] == (char)'L'&&p[f + 5] == (char)'I'&&p[f + 6] == (char)'F'&&p[f + 7] == (char)'F'
										&&p[f + 8] == (char)':'&&p[f + 9] == (char)' '&&p[f + 11] == (char)'>')//</ XLIFF: g>
									{
										szSTR3[len] = (char)'<';
										szSTR3[len + 1] = (char)'/';
										szSTR3[len + 2] = (char)'x';
										szSTR3[len + 3] = (char)'l';
										szSTR3[len + 4] = (char)'i';
										szSTR3[len + 5] = (char)'f';
										szSTR3[len + 6] = (char)'f';
										szSTR3[len + 7] = (char)':';
										szSTR3[len + 8] = (char)'g';
										szSTR3[len + 9] = (char)'>';
										len += 10;
										f += 11;
										continue;
									}
									else
										if (p[f + 0] == (char)'<'&&p[f + 1] == (char)'//'&&p[f + 2] == (char)' '&&p[f + 3] == (char)'X'
											&&p[f + 4] == (char)'L'&&p[f + 5] == (char)'I'&&p[f + 6] == (char)'F'&&p[f + 7] == (char)'F'
											&&p[f + 8] == (char)':'&&p[f + 9] == (char)' '&&p[f + 12] == (char)'>')//</ XLIFF: ã> //rus
										{
											szSTR3[len] = (char)'<';
											szSTR3[len + 1] = (char)'/';
											szSTR3[len + 2] = (char)'x';
											szSTR3[len + 3] = (char)'l';
											szSTR3[len + 4] = (char)'i';
											szSTR3[len + 5] = (char)'f';
											szSTR3[len + 6] = (char)'f';
											szSTR3[len + 7] = (char)':';
											szSTR3[len + 8] = (char)'g';
											szSTR3[len + 9] = (char)'>';
											len += 10;
											f += 12;
											continue;
										}
										else
											if (p[f + 0] == (char)'<'&&p[f + 1] == (char)'//'&&p[f + 2] == (char)' '&&p[f + 3] == (char)'x'
												&&p[f + 4] == (char)'l'&&p[f + 5] == (char)'i'&&p[f + 6] == (char)'f'&&p[f + 7] == (char)'f'
												&&p[f + 8] == (char)':'&&p[f + 9] == (char)' '&&p[f + 12] == (char)'>')//</ xliff: g>
											{
												szSTR3[len] = (char)'<';
												szSTR3[len + 1] = (char)'/';
												szSTR3[len + 2] = (char)'x';
												szSTR3[len + 3] = (char)'l';
												szSTR3[len + 4] = (char)'i';
												szSTR3[len + 5] = (char)'f';
												szSTR3[len + 6] = (char)'f';
												szSTR3[len + 7] = (char)':';
												szSTR3[len + 8] = (char)'g';
												szSTR3[len + 9] = (char)'>';
												len += 10;
												f += 12;
												continue;
											}
											else
												if (p[f + 0] == (char)'<'&&p[f + 1] == (char)'//'&&p[f + 2] == (char)' '&&p[f + 3] == (char)'x'
													&&p[f + 4] == (char)'l'&&p[f + 5] == (char)'i'&&p[f + 6] == (char)'f'&&p[f + 7] == (char)'f'
													&&p[f + 8] == (char)':'&&p[f + 9] == (char)' '&&p[f + 10] == (char)'g' &&p[f + 11] == (char)'>')
													//	&&p[f+12]==(char)'&'&&p[f+13]==(char)'g'&&p[f+14]==(char)'t')// </ xliff: g%gt  /ger
												{
													szSTR3[len] = (char)'<';
													szSTR3[len + 1] = (char)'/';
													szSTR3[len + 2] = (char)'x';
													szSTR3[len + 3] = (char)'l';
													szSTR3[len + 4] = (char)'i';
													szSTR3[len + 5] = (char)'f';
													szSTR3[len + 6] = (char)'f';
													szSTR3[len + 7] = (char)':';
													szSTR3[len + 8] = (char)'g';
													szSTR3[len + 9] = (char)'>';
													len += 10;
													f += 12;
													continue;
												}


												else
													if (p[f + 0] == (char)'<'&&p[f + 1] == (char)'//'&&p[f + 2] == (char)' '&&p[f + 3] == (char)'X'
														&&p[f + 4] == (char)'L'&&p[f + 5] == (char)'I'&&p[f + 6] == (char)'F'&&p[f + 7] == (char)'F'
														&&p[f + 8] == (char)0xEF && p[f + 9] == (char)0xbc && p[f + 10] == (char)0x9a && p[f + 11] == (char)'G'&&p[f + 12] == (char)'>')//	</ XLIFF?G> //jap
													{
														szSTR3[len] = (char)'<';
														szSTR3[len + 1] = (char)'/';
														szSTR3[len + 2] = (char)'x';
														szSTR3[len + 3] = (char)'l';
														szSTR3[len + 4] = (char)'i';
														szSTR3[len + 5] = (char)'f';
														szSTR3[len + 6] = (char)'f';
														szSTR3[len + 7] = (char)':';
														szSTR3[len + 8] = (char)'g';
														szSTR3[len + 9] = (char)'>';
														len += 10;
														f += 12;
														continue;
													}

													else
													{
														szSTR3[len] = (char)(p[f]);
														len++;
													}


						}
						else
							if (p[f + 0] == (char)' '&&p[f + 1] == (char)'.'&&p[f + 2] == (char)'.' &&p[f + 3] == (char)'.')				// " ..."
							{
								szSTR3[len] = '.';
								szSTR3[len + 1] = '.';
								szSTR3[len + 2] = '.';

								len++;len++; len++;
								f += 3;
								continue;
							}

							else
								if (p[f + 0] == (char)'I'&&p[f + 1] == (char)'d'&&p[f + 2] == (char)'='
									&&p[f + 3] == (char)'"'&&p[f + 4] == (char)'n'&&p[f + 5] == (char)'u')				// Id="number"
								{
									szSTR3[len] = 'i';
									szSTR3[len + 1] = 'd';
									szSTR3[len + 2] = '=';//id="

									len++;len++; len++;
									f += 2;
									continue;
								}
								else
									if (p[f + 0] == '['&&p[f + 1] == ' ' &&p[f + 2] == '['&&p[f + 3] == ' ' &&p[f + 4] == '['&&p[f + 5] == ' ') //[[[XXXXX]]]
									{
										//szSTR3[len]=(char)' ';len++;
										szSTR3[len] = (char)'{';len++;


										f += 5;
									}
									else
										if (p[f + 0] == '['&&p[f + 1] == ' ' &&p[f + 2] == '['&&p[f + 3] == ' ' &&p[f + 4] == '[') //[[[XXXXX]]]
										{
											szSTR3[len] = (char)'{';// 

											len++;
											f += 4;
										}
										else
											if (p[f + 0] == ' '&&p[f + 1] == ']' &&p[f + 2] == ' '&&p[f + 3] == ']' &&p[f + 4] == ' '&&p[f + 5] == ']') //[[[XXXXX ] ] ]
											{
												szSTR3[len] = (char)'}';// 

												len++;
												f += 5;
											}
											else
												if (p[f + 0] == ']'&&p[f + 1] == ' ' &&p[f + 2] == ']'&&p[f + 3] == ' ' &&p[f + 4] == ']') //[[[XXXXX] ] ]
												{
													szSTR3[len] = (char)'}';// 

													len++;
													f += 4;
												}
												else
													if (p[f + 0] == '['&&p[f + 1] == ' ' &&p[f + 2] == 'I'
														&&p[f + 3] >= '0'&&p[f + 3] <= '9'
														&&p[f + 4] >= '0'&&p[f + 4] <= '9'
														&&p[f + 5] >= '0'&&p[f + 5] <= '9'
														&&p[f + 6] == ' '&&p[f + 7] == ']') //[ IXXX ]
													{
														szSTR3[len] = (char)'[';	len++;//
														szSTR3[len] = (char)p[f + 2];	len++;//
														szSTR3[len] = (char)p[f + 3];	len++;//
														szSTR3[len] = (char)p[f + 4];	len++;//
														szSTR3[len] = (char)p[f + 5];	len++;//
														szSTR3[len] = (char)']';	len++;//


														f += 7;
													}
													else

														if (p[f + 0] == ' '&&p[f + 1] == ')' &&p[f + 2] == ' ') // "( 1 ) "
														{
															szSTR3[len] = (char)')';len++;
															szSTR3[len] = (char)' ';len++;

															f += 2;
														}

														else
															if (p[f + 0] == ' '&&p[f + 1] == '(' &&p[f + 2] == ' ') //" ( 1 ) "
															{
																szSTR3[len] = (char)' ';len++;
																szSTR3[len] = (char)'(';len++;

																f += 2;
															}
															else
																if (p[f + 0] == ' '&&p[f + 1] == ')') // ( 1 )
																{
																	szSTR3[len] = (char)')';len++;
																	szSTR3[len] = (char)' ';len++;

																	f += 1;
																}
																else
																	if (p[f + 0] == '('&&p[f + 1] == ' ') // ( 1 )
																	{
																		szSTR3[len] = (char)' ';len++;
																		szSTR3[len] = (char)'(';len++;

																		f += 1;
																	}
																	else
																		if (p[f + 0] == ' '&&p[f + 1] == '.') // ( 1 )
																		{
																			szSTR3[len] = (char)'.';len++;
																			szSTR3[len] = (char)' ';len++;

																			f += 1;
																		}
																		else


																			if (p[f + 0] == (char)0x0A) //yahoo
																			{
																				//szSTR3[len]=0x0A;
																				szSTR3[len] = 0x0d;
																				szSTR3[len + 1] = 0x0A;
																				len++;len++;
																				//f+=1;
																				continue;
																			}
																			else
																			{
																				szSTR3[len] = (char)(p[f]);
																				len++;
																			}

	szSTR3[szSTR3.size()] = 0;

	if (len>2)
		if (szSTR3[len - 1] == '.' && szSTR3[len - 2] == ' ') { szSTR3[len - 1] = 0;szSTR3[len - 2] = '.'; }

	p = szSTR3;
	lengh = len;


	//------fix quotes-----
	char quote = 0;
	for (f = 0;f<lengh;f++)
	{
		if (p[f] == ' ' && p[f + 1] == 0x27)
		{
			quote = 0x27;
			f++;f++;
			for (f;f<lengh;f++)
			{
				if ((p[f] == 0x27) && (p[f + 1] == ' ' || p[f + 1] == ','))
					break;

				if (p[f] == ' ' &&p[f + 1] == '"')
				{
					p[f] = quote;
					p[f + 1] = ' ';
					quote = 0;
					break;
				}
				if (p[f] == ' ' && (unsigned char)p[f + 1] == 0xC2 && (unsigned char)p[f + 2] == 0xBB)
				{
					p[f] = quote;
					p[f + 1] = ' ';
					p[f + 2] = ' ';
					quote = 0;
					break;
				}
			}
		}
		if (p[f] == ' ' && p[f + 1] == '"')
		{
			quote = '"';
			f++;f++;
			for (f;f<lengh;f++)
			{
				if (p[f] == '"' && (p[f + 1] == ' ' || p[f + 1] == ','))
					break;

				if (p[f] == ' ' &&p[f + 1] == '"')
				{
					p[f] = quote;
					p[f + 1] = ' ';
					quote = 0;
					break;
				}
				if ((unsigned char)p[f] == 0xC2 && (unsigned char)p[f + 1] == 0xBB)
				{
					p[f] = quote;
					p[f + 1] = ' ';
					quote = 0;
					break;
				}
			}
		}
	}
	//------fix quotes-----

	//------delete double spaces-----
	for (f = 0;f<lengh;f++)

		if (p[f] == ' ' && (p[f + 1] == ' ' || p[f + 1] == ','))
			for (int d = f;d<lengh;d++)
				p[d] = p[d + 1];
	//---------------



}
//It is not my code -- end
bool check(string& str)
{
	stringstream stream;
	stream << str;
	string ss;
	stream >> ss;
	return ss == "<string";
}

int main(int argc, char *argv[]) 
{
	if(argc<=1) { cout << "cantt"; return 1; }
	string file = argv[1];
	ifstream in(file);
	//if (in) { cout << "cant"; return 1; }
	ofstream out(file +"1");
	string st;
	int sch = 0;
	while (!in.eof())
	{
		getline(in, st);
		if (check(st))
		{
			sch++;
			int l = 0;
			for (;st[l] != '>';l++);
			if (st.substr(l - 1, 2) =="/>")
				goto label;
			l++;
			int r = l;
			for (;st[r] != '<';r++)
			{
				if (r == st.size()) {
					string t;
					getline(in, t);
					st += "\n" + t;
				}
			}
			string str = st.substr(l, r - l);
			string res = "";
			int start = 0;
			for (int i = 0;i < str.size();i++)
			{
				if (str[i] == '\n')
				{
					string t = str.substr(start,i-start);
					Deconvert_single_unicode_symbols(t);
					DeConvert_str(t);
					DeConvert_strGoogle(t);
					t.resize(find(t.begin(), t.end(), '\0')- t.begin());
					res += t + '\n';
					start = i;
				}
			}
			if(res=="")  Deconvert_single_unicode_symbols(str), DeConvert_str(str), DeConvert_strGoogle(str),  res=str;
			st.replace(l, r - l, res.c_str());
		}
		label:
		out << st << endl;
	}
	return 0;
}
