/*************************************************************************
*	System		: POPLAR/ANZU_L
*	File Name	: LST_CMD.C
*	Author		: 桑原 美紀
*	Date		: 1997/03/10
*	RealTimeOS	: ＲＩＳＣ（ＳＨ７０４３）モニター
*	Description	: 通信予約リスト
*	Maintenance	:
*
*	Copyright (C) 1996 Murata Machinery,Ltd. All rights reserved.
*************************************************************************/
#include	"\src\atlanta\define\product.h"
#include	"\src\atlanta\define\apl_msg.h"
#include	"\src\atlanta\define\std.h"

#include	"\src\atlanta\define\cmn_def.h"
#include	"\src\atlanta\define\lst_def.h"

#include	"\src\atlanta\define\cmn_pro.h"
#include	"\src\atlanta\define\lst_pro.h"

#include	"\src\atlanta\ext_v\bkupram.h"
#include	"\src\atlanta\ext_v\lst_data.h"
#include	"\src\atlanta\ext_v\lst_wrd.h"

#if (PRO_BATCH_TX_RESERVE_LIST == ENABLE) && (PRO_BATCH_TX == ENABLE)	/* Add By H.Fujimura 1998/12/01 */
 #if (PRO_PANEL == ANK)
#include	"\src\atlanta\opr\ank\define\opr_def.h"
#include	"\src\atlanta\opr\ank\define\opr_pro.h"
 #endif
 #if (PRO_PANEL == KANJI)
#include	"\src\atlanta\opr\kanji\define\opr_def.h"
#include	"\src\atlanta\opr\kanji\define\opr_pro.h"
 #endif
#endif	/* (PRO_BATCH_TX_RESERVE_LIST == ENABLE) && (PRO_BATCH_TX == ENABLE) */

#if (PRO_OKI == ENABLE)	/* by K.Watanabe 1999/02/09 */
#include	"\src\atlanta\opr\ank\ext_v\opr_wrd0.h"
#endif

/*************************************************************************
	module		:[ツウシンヨヤクリスト印刷]
	function	:[ツウシンヨヤクリストを印刷する]
	common		:[
		List			:
		SYB_CommandFile	:
	]
	condition	:[]
	commment	:[1996/05/30	１ページに収まらなかった時の処理を追加	By H.Hirao
	]
	return		:[印刷結果]
	machine		:[SH7043]
	language	:[SHC]
	keyword		:[LST]
	date		:[93/11/25]
	author		:[増田次郎]
*************************************************************************/
#if (PRO_BATCH_TX_RESERVE_LIST == ENABLE) && (PRO_BATCH_TX == ENABLE)	/* Add By H.Fujimura 1998/12/01 */
WORD 				Print_ReserveCmd_List(
						 void )				/** なし */
{
	UBYTE	com_number;						/** loop */
	UBYTE	box_number;						/** loop */
	register WORD	ret;					/** 印刷結果 */
	UBYTE	page;					/** ページ */
	UBYTE	line_cnt;				/** 残り印刷可能行数 */
	UBYTE	opt_line;				/** ｵﾌﾟｼｮﾝﾃﾞｰﾀ行数*/
	UBYTE	l;

	page = 1;

	for ( com_number = 0; com_number < SYS_COM_FILE_MAX; com_number++ ) {
		if ( SYB_CommandFile[com_number].Status != SYS_CMD_EMPTY ) {
			break;
		}
	}
	for ( box_number = 0; box_number < SYS_BATCH_FILE_MAX; box_number++ ) {
		if ( SYB_BatchTxFile[box_number].Status != SYS_CMD_EMPTY ) {
			break;
		}
	}

	if ( com_number == SYS_COM_FILE_MAX && box_number == SYS_BATCH_FILE_MAX ) {
		return ( MSG_PRN_LST_NO_LIST );
	}

	com_number = 0;
	box_number = 0;
	l = 0;
	opt_line	= 0;				/* 97/09/05 Add By M.Kuwahara */
	DialCheckPointer	= SYS_EMPTY_DIAL;
	DialCheckBitPointer	= 0;

	while ( com_number != SYS_COM_FILE_MAX || box_number != SYS_BATCH_FILE_MAX ) {
		if ( (ret = CharacterPrint( LST_OPEN )) != OK ) {
			return ( ret );
		}

		if ( page == 1 ) {
			line_cnt = (UBYTE)(PaperSizeLen() - 11); /** 印刷可能行数ｾｯﾄ */

			/** １改行＋発信元名、番号印刷＋２改行＋見出し＋１改行 */
#if ( PRO_PANEL == KANJI ) || ( PRO_JIS_CODE == ENABLE )	/* 追加 97/02/12 M.Kuwahara */
			if ( (ret = Print_Head1_Common( PWRD_ComReservedList_K,
								  	  	  LST_NOT, 20-1 )) != OK ) {
				return ( ret );
			}
#else
			if ( (ret = Print_Head1_Common( PWRD_ComReservedList,
	  	  	  LST_XXX_TWI_XXX, 20-1 )) != OK ) {
				return ( ret );
			}
#endif
			/** ページ 現在時間 */
			if ( (ret = PageTimePrint( page, 2-1, 60-1 )) != OK ) {
				return ( ret );
			}

			/** �ｮ�｢�ﾎ�｢�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ｲ */
			Print_ReserveCmd_List_Line( 0, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}

			/** �､NO.�､ﾀﾞｲﾔﾙﾊﾞﾝｺﾞｳ�､ｼﾃｲｼﾞｺｸ�､ｵﾌﾟｼｮﾝ�､ﾋﾞｺｳ�､ */
			Print_ReserveCmd_List_Line( 1, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}
		}
		else {	/**	2ページ目以降	*/
			line_cnt = (UBYTE)(PaperSizeLen() - 7); /** 印刷可能行数ｾｯﾄ */

			/*２行印字（改行）*/
			if ( (ret = CharacterPrint( LST_LINE_FEED +2)) != OK ) {
				return ( ret );
			}

			/** ページ */
			if ( (ret = PagePrint( page, 2-1 )) != OK ) {
				return ( ret );
			}
		}

		while ( com_number != SYS_COM_FILE_MAX || box_number != SYS_BATCH_FILE_MAX ) {
			if ( com_number != SYS_COM_FILE_MAX ) {
				if ( SYB_CommandFile[com_number].Status == SYS_CMD_WAITING
				  || SYB_CommandFile[com_number].Status == SYS_CMD_EXECUTE ) {
					if ((DialCheckPointer == SYS_EMPTY_DIAL) &&	/* 97/09/03 By M.Kuwahara */
					    (opt_line == 0)) {
						DialCheckPointer    = SYS_DIRECT_DIAL;	/** 種別ポインタを初期化   */
						DialCheckBitPointer = 0;				/** ビットポインタを初期化 */
						opt_line = ProgOptionDataToString(
						  SYB_CommandFile[com_number].Option.Item,
						  SYB_CommandFile[com_number].Option.OptionNumber.SecureNumber,
						  SYB_CommandFile[com_number].Option.OptionNumber.RelayGroupNumber,
						  SYB_CommandFile[com_number].Option.OptionNumber.DatabaseNumber,
						  List.S.Work, NULL );										/** オプションデータ行数セット */
					}
					/*�､ Ｘ �､xxxxxxxxxxxx�､Z9,Z9:99�､xxxxxx�､zzzz,zzzz,zzzz,zzzz�､*/
					if ( (ret = Print_ReserveCmd_List_Specifi( page, com_number, &l, &line_cnt )) != OK ) {
																	/** 1明細印刷 */
						return ( ret );
					}
					if ( opt_line <= l ){		/* 備考欄をすべて出力した時 */
						opt_line	= 0;
					}
					else{
						opt_line	-= l;
					}
					if ( line_cnt <= 1 ) {
						if ((DialCheckPointer == SYS_EMPTY_DIAL) &&
						    (opt_line == 0)) {
							com_number++;
							l	= 0;
							for ( ; com_number < SYS_COM_FILE_MAX; com_number++ ){		/* 次のｺﾏﾝﾄﾞﾌｧｲﾙまで */
								if ( SYB_CommandFile[com_number].Status == SYS_CMD_WAITING
								  || SYB_CommandFile[com_number].Status == SYS_CMD_EXECUTE ) {
									break;
								}
							}
							break;
						}
						if ( line_cnt <= 0 ){
							break;
						}
					}
				}
				com_number++;
				l = 0;
			}
			else {
				if ( SYB_BatchTxFile[box_number].Status == SYS_CMD_WAITING
				  || SYB_BatchTxFile[box_number].Status == SYS_CMD_EXECUTE ) {
					/*�､ Ｘ �､xxxxxxxxxxxx�､Z9,Z9:99�､xxxxxx�､zzzz,zzzz,zzzz,zzzz�､*/
					if ( (ret = PrintReserveCmdBatchLine( page, box_number, &line_cnt )) != OK ) {
																	/** 1明細印刷（一括送信用）*/
						return ( ret );
					}
					if ( line_cnt <= 1 ) {
						box_number++;
						for ( ; box_number < SYS_BATCH_FILE_MAX; box_number++ ){		/* 次のﾎﾞｯｸｽﾌｧｲﾙまで */
							if ( SYB_BatchTxFile[box_number].Status == SYS_CMD_WAITING
							  || SYB_BatchTxFile[box_number].Status == SYS_CMD_EXECUTE ) {
								break;
							}
						}
						break;
					}
				}
				box_number++;
			}
		}

		/*�ｶ�｢�ﾖ�｢�ﾖ�｢�｢�ﾖ�｢�｢�｢�ﾖ�｢�｢�ｺ*/
		Print_ReserveCmd_List_Line( 4, List.S.Print, List.S.Attribute );
		if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
			return ( ret );
		}

		/*１行印字（改行）*/
		if ( (ret = CharacterPrint( LST_LINE_FEED )) != OK ) {
			return ( ret );
		}

		if ( (ret = CharacterPrint( LST_CLOSE )) != OK ) {
			return ( ret );
		}
		page++;
	}
		return ( OK );
}

#else	/* (PRO_BATCH_TX_RESERVE_LIST == DISABLE) || (PRO_BATCH_TX == ENABLE) */

WORD 				Print_ReserveCmd_List(
						 void )				/** なし */
{
	UBYTE	i;						/** loop */
	register WORD	ret;					/** 印刷結果 */
	UBYTE	page;					/** ページ */
	UBYTE	line_cnt;				/** 残り印刷可能行数 */
/*	UBYTE	prog_line;				/** ﾌﾟﾛｸﾞﾗﾑﾃﾞｰﾀ行数*/
	UBYTE	opt_line;				/** ｵﾌﾟｼｮﾝﾃﾞｰﾀ行数*/
	UBYTE	l;

	page = 1;

	for ( i = 0; i < SYS_COM_FILE_MAX; i++ ) {
		if ( SYB_CommandFile[i].Status != SYS_CMD_EMPTY ) {
			break;
		}
	}

	if ( i == SYS_COM_FILE_MAX ) {
		return ( MSG_PRN_LST_NO_LIST );
	}

	i = 0;
	l = 0;
	opt_line	= 0;				/* 97/09/05 Add By M.Kuwahara */
	DialCheckPointer	= SYS_EMPTY_DIAL;
	DialCheckBitPointer	= 0;

	while ( i != SYS_COM_FILE_MAX ) {
		if ( (ret = CharacterPrint( LST_OPEN )) != OK ) {
			return ( ret );
		}

		if ( page == 1 ) {
			line_cnt = (UBYTE)(PaperSizeLen() - 11); /** 印刷可能行数ｾｯﾄ */

			/** １改行＋発信元名、番号印刷＋２改行＋見出し＋１改行 */
#if ( PRO_PANEL == KANJI ) || ( PRO_JIS_CODE == ENABLE )	/* 追加 97/02/12 M.Kuwahara */
			if ( (ret = Print_Head1_Common( PWRD_ComReservedList_K,
								  	  	  LST_NOT, 20-1 )) != OK ) {
				return ( ret );
			}
#else
			if ( (ret = Print_Head1_Common( PWRD_ComReservedList,
	  	  	  LST_XXX_TWI_XXX, 20-1 )) != OK ) {
				return ( ret );
			}
#endif
			/** ページ 現在時間 */
			if ( (ret = PageTimePrint( page, 2-1, 60-1 )) != OK ) {
				return ( ret );
			}

			/** �ｮ�｢�ﾎ�｢�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ｲ */
			Print_ReserveCmd_List_Line( 0, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}

			/** �､NO.�､ﾀﾞｲﾔﾙﾊﾞﾝｺﾞｳ�､ｼﾃｲｼﾞｺｸ�､ｵﾌﾟｼｮﾝ�､ﾋﾞｺｳ�､ */
			Print_ReserveCmd_List_Line( 1, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}
		}
		else {	/**	2ページ目以降	*/
			line_cnt = (UBYTE)(PaperSizeLen() - 7); /** 印刷可能行数ｾｯﾄ */

			/*２行印字（改行）*/
			if ( (ret = CharacterPrint( LST_LINE_FEED +2)) != OK ) {
				return ( ret );
			}

			/** ページ */
			if ( (ret = PagePrint( page, 2-1 )) != OK ) {
				return ( ret );
			}
#if (0)
			/** �ｮ�｢�ﾎ�｢�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ｲ */
			Print_ReserveCmd_List_Line( 0, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}
#endif
		}

/*		for ( ; i < SYS_COM_FILE_MAX; i++ ) {*/
		while ( i != SYS_COM_FILE_MAX ) {
			if ( SYB_CommandFile[i].Status == SYS_CMD_WAITING
	 		  || SYB_CommandFile[i].Status == SYS_CMD_EXECUTE ) {
				if ((DialCheckPointer == SYS_EMPTY_DIAL) &&	/* 97/09/03 By M.Kuwahara */
				    (opt_line == 0)) {
#if (0)
					prog_line = ProgramDataDialToString(
					  (struct SYS_DialData_t *)&SYB_CommandFile[i].DialNumber,
					  SYS_PRG_DIAL_MAX, List.S.Work, NULL );					/** プログラムデータ行数セット */
#endif
					DialCheckPointer    = SYS_DIRECT_DIAL;	/** 種別ポインタを初期化   */
					DialCheckBitPointer = 0;				/** ビットポインタを初期化 */
					opt_line = ProgOptionDataToString(
					  SYB_CommandFile[i].Option.Item,
					  SYB_CommandFile[i].Option.OptionNumber.SecureNumber,
					  SYB_CommandFile[i].Option.OptionNumber.RelayGroupNumber,
					  SYB_CommandFile[i].Option.OptionNumber.DatabaseNumber,
					  List.S.Work, NULL );										/** オプションデータ行数セット */
				}
#if (0)
				if ( line_cnt < (UBYTE)(prog_line + 1) || line_cnt < (UBYTE)(opt_line + 1) ) { /** 印刷予定行数が印刷可能か判断 */
					break;
				}
				else {
					if ( line_cnt != (UBYTE)(PaperSizeLen() - 7) ){	/**	2ページ目以降の最初だけ印字しない	*/
						/*�ｾ�｢�ﾞ�｢�ﾞ�｢�｢�ﾞ�｢�｢�｢�ﾞ�｢�｢�ﾆ*/
						Print_ReserveCmd_List_Line( 2, List.S.Print, List.S.Attribute );
						if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
							return ( ret );
						}
					}

					if ( prog_line < opt_line ) { /** 残り行数をセット */
						line_cnt -= (opt_line + 1);	/**	1をプラスしているのは罫線分	*/
					} else {
						line_cnt -= (prog_line + 1);
					}
				}
#endif
				/*�､ Ｘ �､xxxxxxxxxxxx�､Z9,Z9:99�､xxxxxx�､zzzz,zzzz,zzzz,zzzz�､*/
/*				if ( (ret = Print_ReserveCmd_List_Specifi( i )) != OK ) {*/
				if ( (ret = Print_ReserveCmd_List_Specifi( page, i, &l, &line_cnt )) != OK ) {
																/** 1明細印刷 */
					return ( ret );
				}
				if ( opt_line <= l ){		/* 備考欄をすべて出力した時 */
					opt_line	= 0;
				}
				else{
					opt_line	-= l;
				}
				if ( line_cnt <= 1 ) {
					if ((DialCheckPointer == SYS_EMPTY_DIAL) &&
					    (opt_line == 0)) {
						i++;
						l	= 0;
						for ( ; i < SYS_COM_FILE_MAX; i++ ){		/* 次のｺﾏﾝﾄﾞﾌｧｲﾙまで */
							if ( SYB_CommandFile[i].Status == SYS_CMD_WAITING
					 		  || SYB_CommandFile[i].Status == SYS_CMD_EXECUTE ) {
								break;
							}
						}
						break;
					}
					if ( line_cnt <= 0 ){
						break;
					}
				}
			}
			i++;
			l = 0;
		}

		/*�ｶ�｢�ﾖ�｢�ﾖ�｢�｢�ﾖ�｢�｢�｢�ﾖ�｢�｢�ｺ*/
		Print_ReserveCmd_List_Line( 4, List.S.Print, List.S.Attribute );
		if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
			return ( ret );
		}

		/*１行印字（改行）*/
		if ( (ret = CharacterPrint( LST_LINE_FEED )) != OK ) {
			return ( ret );
		}

		if ( (ret = CharacterPrint( LST_CLOSE )) != OK ) {
			return ( ret );
		}
		page++;
	}
		return ( OK );
}
#endif	/* (PRO_BATCH_TX_RESERVE_LIST == DISABLE) || (PRO_BATCH_TX == ENABLE) */

/*************************************************************************
	module		:[ツウシンヨヤクリスト明細印刷]
	function	:[ツウシンヨヤクリストの明細部分を印刷する]
	common		:[
		SYB_CommandFile	:
	]
	condition	:[]
	commment	:[]
	return		:[印刷結果]
	machine		:[SH043]
	language	:[SHC]
	keyword		:[LST]
	date		:[93/11/25]
	author		:[増田次郎]
*************************************************************************/
WORD 				Print_ReserveCmd_List_Specifi(
						 UBYTE Page,		/*ページ*/
						 UBYTE ComNumber,	/*通信予約ﾃﾞｰﾀ番号*/
						 UBYTE *Line,		/* 現在明細数 */
						 UBYTE *LineCnt)	/* 印字行位置 */
{
/*	UBYTE	prog_line;				/*ﾌﾟﾛｸﾞﾗﾑﾃﾞｰﾀ行数*/
	UBYTE	opt_line;				/*ｵﾌﾟｼｮﾝﾃﾞｰﾀ行数*/
/*	UBYTE	j;						/*loop*/
	UBYTE	command_set = FALSE;	/* 通信予約No.ｾｯﾄﾌﾗｸﾞ */
	UBYTE	now_line;
	register WORD	ret;					/*印刷結果*/
/*  UDWORD			test;					/*test*/
#if 0	/* プレフィックス＋ダイヤル番号で40桁を超えても折り返して印字しないので、大島さん確認の上、リストでの展開は止めます by K.Watanabe 2004/11/12 */
//#if defined(STOCKHM2) || (PRO_PREFIX_KEY == ENABLE)	/* DVT: Added by SMuratec K.W.Q 2004/09/10 *//* by K.Watanabe 2004/10/12 */
//	UWORD	length;
#endif			/*	End of defined(STOCKHM2)	*/

	if (Page > 1) {		/* 97/09/05 By M.Kuwahara */
		if (*LineCnt == (UBYTE)(PaperSizeLen() - 7)) {
			/** �ｮ�｢�ﾎ�｢�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ｲ */
			Print_ReserveCmd_List_Line( 0, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}
			command_set	= TRUE;
		}
		else{
			/*�ｾ�｢�ﾞ�｢�ﾞ�｢�｢�ﾞ�｢�｢�｢�ﾞ�｢�｢�ﾆ*/
			Print_ReserveCmd_List_Line( 2, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}
		}
	}
	else{
		/*�ｾ�｢�ﾞ�｢�ﾞ�｢�｢�ﾞ�｢�｢�｢�ﾞ�｢�｢�ﾆ*/
		Print_ReserveCmd_List_Line( 2, List.S.Print, List.S.Attribute );
		if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
			return ( ret );
		}
	}
	(*LineCnt)--;

	/*�､<Ｘ>�､xxxxxxxxxxxx�､Z9,Z9:99�､xxxxxx�､zzzz,zzzz,zzzz,zzzz�､*/
#if (0)
	prog_line = ProgramDataDialToString(
	  (struct SYS_DialData_t *)&SYB_CommandFile[ComNumber].DialNumber, SYS_PRG_DIAL_MAX, List.S.Work, NULL );
#endif
#if ( PRO_MAKER == FX )
	opt_line = ProgOptionDataToString(
	  SYB_CommandFile[ComNumber].Option.Item,
	  SYB_CommandFile[ComNumber].Option.OptionNumber.FX_Secure.Number,
	  SYB_CommandFile[ComNumber].Option.OptionNumber.FX_Relay.Number,
	  SYB_CommandFile[ComNumber].Option.OptionNumber.DatabaseNumber,
	  List.S.Work, NULL );
#else
	opt_line = ProgOptionDataToString(
	  SYB_CommandFile[ComNumber].Option.Item,
	  SYB_CommandFile[ComNumber].Option.OptionNumber.SecureNumber,
	  SYB_CommandFile[ComNumber].Option.OptionNumber.RelayGroupNumber,
	  SYB_CommandFile[ComNumber].Option.OptionNumber.DatabaseNumber,
	  List.S.Work, NULL );
#endif
	if (opt_line >= *Line){		/* 備考欄がまだある時 */
		opt_line	-= *Line;
	}else{
		opt_line	= 0;
	}

	while (*LineCnt != 0) {		/* 97/09/03 By M.Kuwahara */
/*	for ( j = 0; (j < prog_line || j < opt_line); j++ ) {*/
		if(( DialCheckPointer == SYS_EMPTY_DIAL ) && ( opt_line == 0 )){
			break;
		}
		Print_ReserveCmd_List_Line( 3, List.S.Print, List.S.Attribute );
/*		if ( j == 0 ) {*/
		if (( *Line == 0 ) ||
		    (command_set == TRUE)) {
			/* No. */
#if ((PRO_BATCH_TX_RESERVE_LIST == ENABLE) && (PRO_BATCH_TX == ENABLE)) || defined (KEISATSU)	/* Add By H.Fujimura 1998/12/02 */
																								/* 警察FAX 05/12/06 石橋正和 */
			List.S.Print[2-1] = 'C';
			CMN_UnsignedIntToASC( &List.S.Print[3-1], (UWORD)ComNumber, 2, '0' );
#else
 /* Noが「C00」と印字される。*/
 #if (PRO_KEYPANEL == PANEL_SATSUKI2)  || (PRO_KEYPANEL == PANEL_STOCKHM2) /* Modify by SMuratec K.W.Q 2003/10/21 */
			List.S.Print[2-1] = 'C';
			CMN_UnsignedIntToASC( &List.S.Print[3-1], (UWORD)ComNumber, 2, '0' );
 #else
			/* CMN_UnsignedIntToASC( &List.S.Print[3-1], (UWORD)ComNumber, 1, '0' ); */ /** SYS_COM_FILE_MAXが２０になったので変更 */
			CMN_UnsignedIntToASC( &List.S.Print[3-1], (UWORD)ComNumber, 2, ' ' );
 #endif
#endif
		}
		if (*Line == 0){
			/*ｼﾃｲﾆﾁｼﾞ*/
/*			test = SYB_CommandFile[ComNumber].ExecTime/60;*/
			CriterionMinuteToDayTimeAsc( SYB_CommandFile[ComNumber].ExecTime / 60, LST_DDHHMM, List.S.Work );
#if ( PRO_PANEL == KANJI ) || ( PRO_JIS_CODE == ENABLE )
			CMN_StringCopy( &List.S.Print[47-1], List.S.Work );
			/*ｵｳﾖｳｷﾉｳ*/
			CommunicationKindName( SYB_CommandFile[ComNumber].Option.Item, LST_NAME_2, &List.S.Print[56-1], &List.S.Attribute[56-1] );
#else
			CMN_StringCopy( &List.S.Print[47-1], List.S.Work ); /* 48->47 For ANZU 96/02/13 H.Hirao */
			/*ｵｳﾖｳｷﾉｳ*/
			CommunicationKindName( SYB_CommandFile[ComNumber].Option.Item, LST_NAME_2, &List.S.Print[57-1], &List.S.Attribute[57-1] );
#endif
		}
		command_set	= FALSE;

/*		if ( prog_line > j ) {*/
		now_line	= *Line;
		if ( DialCheckPointer != SYS_EMPTY_DIAL ) {
			/*ﾀﾞｲﾔﾙﾊﾞﾝｺﾞｳ項目ﾃﾞｰﾀ*/
#if (0)
//			ProgramDataDialToString(
//			  (struct SYS_DialData_t *)&SYB_CommandFile[ComNumber].DialNumber, 
//				SYS_PRG_DIAL_MAX, List.S.Work, j );
#endif
			ProgramDataDialToString(
			  (struct SYS_DialData_t *)&SYB_CommandFile[ComNumber].DialNumber, 
				SYS_PRG_DIAL_MAX, List.S.Work, now_line );

#if 0	/* プレフィックス＋ダイヤル番号で40桁を超えても折り返して印字しないので、大島さん確認の上、リストでの展開は止めます by K.Watanabe 2004/11/12 */
//#if defined(STOCKHM2) || (PRO_PREFIX_KEY == ENABLE)	/* DVT: Added by SMuratec K.W.Q 2004/09/09 *//* by K.Watanabe 2004/10/12 */
//			if (List.S.Work[0] == '!'){
//				length = CMN_StringLength(SYB_MercuryDialBuffer);	/* マキュリー登録桁 */
//
//				List.S.Print[5] = '[';
//				if (length > 38){
//					CMN_StringCopyNumUntilNull( &List.S.Print[6], SYB_MercuryDialBuffer, 39);
//				}
//				else{
//					List.S.Print[6 + length] = ']';
//					
//					CMN_StringCopyNumUntilNull( &List.S.Print[6], SYB_MercuryDialBuffer, length);
//					CMN_StringCopyNumUntilNull( &List.S.Print[7 + length], &List.S.Work[1],
//						(38 - length) );
//				}
//			}
//			else{
//				CMN_StringCopy( &List.S.Print[6-1], List.S.Work );
//			}
#else
			CMN_StringCopy( &List.S.Print[6-1], List.S.Work );
#endif		/* End of defined(STOCKHM2) || (PRO_PREFIX_KEY == ENABLE) */
		}
#if (0)
//		if ( opt_line > j ) {
//			/*備考項目ﾃﾞｰﾀ*/
//#if ( PRO_MAKER == FX )
//			ProgOptionDataToString(
//			  SYB_CommandFile[ComNumber].Option.Item,
//			  SYB_CommandFile[ComNumber].Option.OptionNumber.FX_Secure.Number,
//			  SYB_CommandFile[ComNumber].Option.OptionNumber.FX_Relay.Number,
//			  SYB_CommandFile[ComNumber].Option.OptionNumber.DatabaseNumber,
//			  List.S.Work, j );
//#else
//			ProgOptionDataToString(
//			  SYB_CommandFile[ComNumber].Option.Item,
//			  SYB_CommandFile[ComNumber].Option.OptionNumber.SecureNumber,
//			  SYB_CommandFile[ComNumber].Option.OptionNumber.RelayGroupNumber,
//			  SYB_CommandFile[ComNumber].Option.OptionNumber.DatabaseNumber,
//			  List.S.Work, j );
//#endif
//			CMN_StringCopy( &List.S.Print[65-1], List.S.Work );
//		}
#endif	/* (0) */
		if ( opt_line > 0 ) {
			/*備考項目ﾃﾞｰﾀ*/
#if ( PRO_MAKER == FX )
			ProgOptionDataToString(
			  SYB_CommandFile[ComNumber].Option.Item,
			  SYB_CommandFile[ComNumber].Option.OptionNumber.FX_Secure.Number,
			  SYB_CommandFile[ComNumber].Option.OptionNumber.FX_Relay.Number,
			  SYB_CommandFile[ComNumber].Option.OptionNumber.DatabaseNumber,
			  List.S.Work, now_line );
#else
			ProgOptionDataToString(
			  SYB_CommandFile[ComNumber].Option.Item,
			  SYB_CommandFile[ComNumber].Option.OptionNumber.SecureNumber,
			  SYB_CommandFile[ComNumber].Option.OptionNumber.RelayGroupNumber,
			  SYB_CommandFile[ComNumber].Option.OptionNumber.DatabaseNumber,
			  List.S.Work, now_line );
#endif
			CMN_StringCopy( &List.S.Print[65-1], List.S.Work );
			opt_line--;
		}

		if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
			return ( ret );
		}
		(*LineCnt)--;
		(*Line)++;
	}
	return ( OK );
}

/*************************************************************************
	module		:[ツウシンヨヤク横区切り罫線作成]
	functiom	:[
				ツウシンヨヤクの横区切り罫線を作成する。
				]
	common		:[]
	condition	:[
		罫線種類
		,	0 = �ｮ�｢�｢�ﾎ�｢�｢�｢�｢�｢�｢�ﾎ�｢�｢�｢�｢�ﾎ�｢�｢�｢�ﾎ�｢�｢�ｲ
		,	1 = �､NO. �､			�､		  �､	  �､	�､
		,	2 = �ｾ�｢�｢�ﾞ�｢�｢�｢�｢�｢�｢�ﾞ�｢�｢�｢�｢�ﾞ�｢�｢�｢�ﾞ�｢�｢�ﾆ
		,	3 = �､　  �､			�､		  �､	  �､	�､
		,	4 = �ｶ�｢�｢�ﾖ�｢�｢�｢�｢�｢�｢�ﾖ�｢�｢�｢�｢�ﾖ�｢�｢�｢�ﾖ�｢�｢�ｺ
	]
	commment	:[
				ﾌﾟﾛｸﾞﾗﾑﾜﾝﾀｯﾁの罫線が変更した為、作成
				]
	return		:[印刷結果なし]
	machine		:[SH7043]
	language	:[SHC]
	keyword		:[LST]
	date		:[1996/12/28]
	author		:[磯辺毅]
*************************************************************************/
void			Print_ReserveCmd_List_Line(
						 UBYTE Category,				/*罫線種類*/
						 register UBYTE *AscString,		/*印刷文字列*/
						 register UBYTE *AttString )	/*修飾文字列*/
{
	Prn_Att_Str_Clear( AscString, AttString );
	switch ( Category ) {
		case 0:
			/*�ｮ�｢�｢�ﾎ�｢�｢�｢�｢�｢�｢�ﾎ�｢�｢�｢�｢�ﾎ�｢�｢�｢�ﾎ�｢�｢�ｲ*/
			AscString[1-1] = LST_KLINE7;
			CMN_MemorySet( &AscString[2-1], 78, LST_KLINE_ );
			AscString[5-1] = LST_KLINE8;
			AscString[46-1] = LST_KLINE8;
			AscString[55-1] = LST_KLINE8;
			AscString[64-1] = LST_KLINE8;
			AscString[80-1] = LST_KLINE9;
			break;
		case 1:
			/*�､NO. �､ﾀﾞｲﾔﾙ番号 �､指定日時 �､応用機能�､備考�､*/
			AscString[1-1] = LST_KLINEI;
			AttributeSet( &AscString[2-1], &AttString[2-1], PWRD_No, LST_NOT );
			AscString[5-1] = LST_KLINEI;
			AttributeSet( &AscString[17-1], &AttString[17-1], PWRD_TeleNumber,
			  LST_NOT );	/* 18 -> 17 97/03/11 M.Kuwahara */
			AscString[46-1] = LST_KLINEI;
			AttributeSet( &AscString[47-1], &AttString[47-1], PWRD_DelyTime,
			  LST_NOT );
			AscString[55-1] = LST_KLINEI;
#if ( PRO_PANEL == KANJI ) || ( PRO_JIS_CODE == ENABLE )	/* 追加 97/02/12 M.Kuwahara */
			AttributeSet( &AscString[56-1], &AttString[56-1], PWRD_Option,
			  LST_NOT );
#else
			AttributeSet( &AscString[56-1], &AttString[56-1], PWRD_Option,
			  LST_NOT );	/* 57->56に変更 98/02/16 Y.Matsukuma */
#endif
			AscString[64-1] = LST_KLINEI;
			AttributeSet( &AscString[69-1], &AttString[69-1], PWRD_Note,
			  LST_NOT );
			AscString[80-1] = LST_KLINEI;
			break;
		case 2:
			/*�ｾ�｢�｢�ﾞ�｢�｢�｢�｢�｢�｢�ﾞ�｢�｢�｢�｢�ﾞ�｢�｢�｢�ﾞ�｢�｢�ﾆ*/
			AscString[1-1] = LST_KLINE4;
			CMN_MemorySet( &AscString[2-1], 78, LST_KLINE_ );
			AscString[5-1] = LST_KLINE5;
			AscString[46-1] = LST_KLINE5;
			AscString[55-1] = LST_KLINE5;
			AscString[64-1] = LST_KLINE5;
			AscString[80-1] = LST_KLINE6;
			break;
		case 3:
			/*�､　	�､			  �､		�､		�､	  �､*/
			AscString[1-1] = LST_KLINEI;
			AscString[5-1] = LST_KLINEI;
			AscString[46-1] = LST_KLINEI;
			AscString[55-1] = LST_KLINEI;
			AscString[64-1] = LST_KLINEI;
			AscString[80-1] = LST_KLINEI;
			break;
		case 4:
			/*�ｶ�｢�｢�ﾖ�｢�｢�｢�｢�｢�｢�ﾖ�｢�｢�｢�｢�ﾖ�｢�｢�｢�ﾖ�｢�｢�ｺ*/
			AscString[1-1] = LST_KLINE1;
			CMN_MemorySet( &AscString[2-1], 78, LST_KLINE_ );
			AscString[5-1] = LST_KLINE2;
			AscString[46-1] = LST_KLINE2;
			AscString[55-1] = LST_KLINE2;
			AscString[64-1] = LST_KLINE2;
			AscString[80-1] = LST_KLINE3;
			break;
	}
}

#if (PRO_BATCH_TX_RESERVE_LIST == ENABLE) && (PRO_BATCH_TX == ENABLE)	/* Add By H.Fujimura 1998/12/01 */
/*************************************************************************
	module		:[ツウシンヨヤクリスト明細印刷（一括送信用）]
	function	:[ツウシンヨヤクリストの明細部分（一括送信用）を印刷する]
	common		:[
		SYB_CommandFile	:
	]
	condition	:[]
	commment	:[]
	return		:[印刷結果]
	machine		:[SH043]
	language	:[SHC]
	keyword		:[LST]
	date		:[1998/12/01]
	author		:[藤村春夫]
*************************************************************************/
WORD 				PrintReserveCmdBatchLine(
						 UBYTE Page,		/*ページ*/
						 UBYTE BoxNumber,	/*一括送信ボックス番号*/
						 UBYTE *LineCnt)	/* 印字行位置 */
{
	register WORD	ret;					/*印刷結果*/

	if (Page > 1) {		/* 97/09/05 By M.Kuwahara */
		if (*LineCnt == (UBYTE)(PaperSizeLen() - 7)) {
			/** �ｮ�｢�ﾎ�｢�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ｲ */
			Print_ReserveCmd_List_Line( 0, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}
		}
		else{
			/*�ｾ�｢�ﾞ�｢�ﾞ�｢�｢�ﾞ�｢�｢�｢�ﾞ�｢�｢�ﾆ*/
			Print_ReserveCmd_List_Line( 2, List.S.Print, List.S.Attribute );
			if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
				return ( ret );
			}
		}
	}
	else{
		/*�ｾ�｢�ﾞ�｢�ﾞ�｢�｢�ﾞ�｢�｢�｢�ﾞ�｢�｢�ﾆ*/
		Print_ReserveCmd_List_Line( 2, List.S.Print, List.S.Attribute );
		if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
			return ( ret );
		}
	}
	(*LineCnt)--;

	/*�､<Ｘ>�､xxxxxxxxxxxx�､Z9,Z9:99�､xxxxxx�､zzzz,zzzz,zzzz,zzzz�､*/
	Print_ReserveCmd_List_Line( 3, List.S.Print, List.S.Attribute );

	/* No. */
	List.S.Print[2-1] = 'B';
	CMN_UnsignedIntToASC( &List.S.Print[3-1], (UWORD)(BoxNumber + 1),
	  2, '0' );

	/*ｼﾃｲﾆﾁｼﾞ*/
	CriterionMinuteToDayTimeAsc( SYB_BatchTxFile[BoxNumber].ExecTime
	  /60, LST_DDHHMM, List.S.Work );
 #if ( PRO_PANEL == KANJI ) || ( PRO_JIS_CODE == ENABLE )
	CMN_StringCopy( &List.S.Print[47-1], List.S.Work );
	/*ｵｳﾖｳｷﾉｳ*/
	CommunicationKindName( SYS_BATCH_MEMORY_TX,
	  LST_NAME_2, &List.S.Print[56-1], &List.S.Attribute[56-1] );
 #else
	CMN_StringCopy( &List.S.Print[47-1], List.S.Work ); /* 48->47 For ANZU 96/02/13 H.Hirao */
	/*ｵｳﾖｳｷﾉｳ*/
	CommunicationKindName( SYS_BATCH_MEMORY_TX,
	  LST_NAME_2, &List.S.Print[57-1], &List.S.Attribute[57-1] );
 #endif

	/*ﾀﾞｲﾔﾙﾊﾞﾝｺﾞｳ項目ﾃﾞｰﾀ*/
	switch (SYB_BatchTxFile[BoxNumber].DialNumber[0].Item) {
	case SYS_DIRECT_DIAL:
		CMN_BCD_ToASC_String(List.S.Work,
							 &SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Direct[0]); 
		break;
	case SYS_SPEED_DIAL:
#if (0)
//		List.S.Work[0] = '(';
//		CMN_UnsignedIntToASC(&List.S.Work[1], 
//							 (SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed + OPR_SPEED_DIAL_MIN),
//							 3 ,'0');
//		List.S.Work[4] = ')';
//		List.S.Work[5] = NULL;
//		break;
#endif
		if ( SYS_SPEED_DIAL_MAX+SYS_2ND_SPEED_DIAL_MAX >= 100 ) {
 #if (PRO_OKI == ENABLE)	/* 海外では短縮等の頭文字が英語と違うので、変更できるようにします by K.Watanabe 1999/02/09 */
			List.S.Work[0] = *CalculateStringAddress(&D2W_HeadCharacter_Speed);
 #else
			List.S.Work[0] = 'S';
 #endif
			if( CHK_UNI_OptionalSpeedDial() != 0 ){	/* 任意短縮の時 */
#if (1) /* 任意短縮４桁化 2002/02/13 By M.Maeda */
				if ( GetRecordFromNumber( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed ) != 0xFFFF ) {
					CMN_UnsignedIntToASC( &List.S.Work[1],
										  GetRecordFromNumber( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed ),
										  OP_SPEED_FIGURE_MAX, '0' );
				}
				else {
					CMN_MemorySet( &List.S.Work[1], OP_SPEED_FIGURE_MAX, '*' );
				}
#else
//				if ( GetRecordFromNumber( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed ) != 0xFFFF ) {
//					CMN_UnsignedIntToASC( &List.S.Work[1],
//										  GetRecordFromNumber( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed ),
//										  3, '0' );
//				}
//				else {
//					CMN_MemorySet( &List.S.Work[1], 3,'*' );
//				}
#endif
			}
			else{	/* 通常の短縮の時 */
 #if ( PRO_CPU == SH7043 )	/* FX,MMLで短縮ﾀﾞｲﾔﾙNo.が000〜or001〜と違う時のため */
				CMN_UnsignedIntToASC( &List.S.Work[1], 
									  SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed+(OPR_SPEED_DIAL_MIN), 
									  3, '0' );
 #else
				CMN_UnsignedIntToASC( &List.S.Work[1], 
									  SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed+1,
									  3, '0' );
 #endif
			}
#if (1) /* 任意短縮４桁化 2002/02/13 By M.Maeda */
			if( CHK_UNI_OptionalSpeedDial() != 0 ){	/* 任意短縮の時 */
				List.S.Work[OP_SPEED_FIGURE_MAX + 1] = NULL;
			}
			else {
				List.S.Work[4] = NULL;
			}
#else
//			List.S.Work[4] = NULL;
#endif
		}
		else {
 #if (PRO_OKI == ENABLE)	/* 海外では短縮等の頭文字が英語と違うので、変更できるようにします by K.Watanabe 1999/02/09 */
			List.S.Work[0] = *CalculateStringAddress(&D2W_HeadCharacter_Speed);
 #else
			List.S.Work[0] = 'S';
 #endif
			if( CHK_UNI_OptionalSpeedDial() != 0 ){	/* 任意短縮の時 */
				if ( GetRecordFromNumber( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed ) != 0xFFFF ) {
					CMN_UnsignedIntToASC( &List.S.Work[1], 
						GetRecordFromNumber( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed ), 2, '0' );
				}
				else {
					CMN_MemorySet( &List.S.Work[1], 3,'*' );
				}
			}
			else{	/* 通常の短縮の時 */
 #if ( PRO_CPU == SH7043 )	/* FX,MMLで短縮ﾀﾞｲﾔﾙNo.が000〜or001〜と違う時のため */
				CMN_UnsignedIntToASC( &List.S.Work[1], 
									  SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed,
									  2, '0' );
 #else
				CMN_UnsignedIntToASC( &List.S.Work[1], 
									  SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Speed + 1,
									  2, '0' );
 #endif
			}
			List.S.Work[3] = NULL;
		}
		break;
	case SYS_ONETOUCH_DIAL:
 #if (PRO_KEYPANEL == PANEL_ANZU_L)
		List.S.Work[0] = '[';
		if (SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Onetouch >= CMN_OnetouchCodeToOffset(ONETOUCH_U1_KEY)) {
			List.S.Work[1] = 'U';
			List.S.Work[2] = 
				GetOnetouchSideCode(CMN_OffsetToOnetouchCode(SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Onetouch));
			List.S.Work[3] = ']';
			List.S.Work[4] = NULL;
		}
		else {
			List.S.Work[1] = CMN_OffsetToOnetouchCode(SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Onetouch);
			List.S.Work[2] = ']';
			List.S.Work[3] = NULL;
		}
 #else
		CMN_MemorySet( List.S.Work, 4, ' ' );
		List.S.Work[4] = NULL;
  #if (PRO_PANEL == ANK)
		List.S.Work[0] = '[';
		CMN_UnsignedIntToASC( &List.S.Work[1], 	/* 01〜72記号をつくる */
				(UWORD)(CMN_OffsetToOnetouchCode( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Onetouch ) - 0x40),
				 2, '0' );
		List.S.Work[3] = ']';
  #endif
  #if (PRO_PANEL == KANJI)
  		/* ワンタッチの表示を[]->絵文字に変更 By O.Kimoto 1998/01/22 */
		List.S.Work[0] = LCD_PICFONT;
		List.S.Work[1] = LCD_PICFONT_LOWER;
		CMN_UnsignedIntToASC( &List.S.Work[2], 	/* 01〜72記号をつくる */
				(UWORD)(CMN_OffsetToOnetouchCode( SYB_BatchTxFile[BoxNumber].DialNumber[0].FaxNumber.Onetouch ) - 0x40),
				 2, '0' );
  #endif
 #endif
		break;
	default:
		break;
	}
	CMN_StringCopy( &List.S.Print[6-1], List.S.Work );

	/*備考項目ﾃﾞｰﾀ*/

	if ( (ret = CharacterPrint( LST_PRINT )) != OK ) {
		return ( ret );
	}
	(*LineCnt)--;

	return ( OK );
}
#endif	/* (PRO_BATCH_TX_RESERVE_LIST == ENABLE) && (PRO_BATCH_TX == ENABLE) */
