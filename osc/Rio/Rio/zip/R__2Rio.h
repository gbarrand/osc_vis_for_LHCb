#ifndef R__2Rio_h
#define R__2Rio_h

/*
 * This file is included in zconf.h to avoid
 * namespace clash with other libs (for exa ROOT)
 * that embeds zip too.
 * 
 */

/*
 * The below R__ list had been extracted by doing a :
 *    UNIX> nm libRio.a | grep R__
 * with the original code.
 */

#define R__Deflate Rio_R__Deflate
#define R__Deflate_fast Rio_R__Deflate_fast
#define R__bi_init Rio_R__bi_init
#define R__bi_reverse Rio_R__bi_reverse
#define R__bi_windup Rio_R__bi_windup
#define R__block_start Rio_R__block_start
#define R__build_bl_tree Rio_R__build_bl_tree
#define R__build_tree Rio_R__build_tree
#define R__compress_block Rio_R__compress_block
#define R__copy_block Rio_R__copy_block
#define R__ct_init Rio_R__ct_init
#define R__ct_tally Rio_R__ct_tally
#define R__error Rio_R__error
#define R__file_method Rio_R__file_method
#define R__file_type Rio_R__file_type
#define R__fill_window Rio_R__fill_window
#define R__flush_block Rio_R__flush_block
#define R__flush_outbuf Rio_R__flush_outbuf
#define R__gen_bitlen Rio_R__gen_bitlen
#define R__gen_codes Rio_R__gen_codes
#define R__good_match Rio_R__good_match
#define R__head Rio_R__head
#define R__init_block Rio_R__init_block
#define R__lm_free Rio_R__lm_free
#define R__lm_init Rio_R__lm_init
#define R__longest_match Rio_R__longest_match
#define R__match_start Rio_R__match_start
#define R__max_chain_length Rio_R__max_chain_length
#define R__mem_read Rio_R__mem_read
#define R__memcompress Rio_R__memcompress
#define R__nice_match Rio_R__nice_match
#define R__pqdownheap Rio_R__pqdownheap
#define R__prev Rio_R__prev
#define R__prev_length Rio_R__prev_length
#define R__read_buf Rio_R__read_buf
#define R__scan_tree Rio_R__scan_tree
#define R__seekable Rio_R__seekable
#define R__send_all_trees Rio_R__send_all_trees
#define R__send_bits Rio_R__send_bits
#define R__send_tree Rio_R__send_tree
#define R__set_file_type Rio_R__set_file_type
#define R__strstart Rio_R__strstart
#define R__window Rio_R__window
#define R__window_size Rio_R__window_size
#define R__zip Rio_R__zip
#define R__Inflate Rio_R__Inflate
#define R__Inflate_block Rio_R__Inflate_block
#define R__Inflate_codes Rio_R__Inflate_codes
#define R__Inflate_dynamic Rio_R__Inflate_dynamic
#define R__Inflate_fixed Rio_R__Inflate_fixed
#define R__Inflate_free Rio_R__Inflate_free
#define R__Inflate_stored Rio_R__Inflate_stored
#define R__WriteData Rio_R__WriteData
#define R__fixed_bd Rio_R__fixed_bd
#define R__fixed_bl Rio_R__fixed_bl
#define R__fixed_td Rio_R__fixed_td
#define R__fixed_tl Rio_R__fixed_tl
#define R__huft_build Rio_R__huft_build
#define R__huft_free Rio_R__huft_free
#define R__slide Rio_R__slide
#define R__unzip Rio_R__unzip
#define R__z_errmsg Rio_R__z_errmsg

/* From zconf.h */
#  define deflateInit_  Rio_deflateInit_
#  define deflate       Rio_deflate
#  define deflateEnd    Rio_deflateEnd
#  define inflateInit_  Rio_inflateInit_
#  define inflate       Rio_inflate
#  define inflateEnd    Rio_inflateEnd
#  define deflateInit2_ Rio_deflateInit2_
#  define deflateSetDictionary Rio_deflateSetDictionary
#  define deflateCopy   Rio_deflateCopy
#  define deflateReset  Rio_deflateReset
#  define deflatePrime  Rio_deflatePrime
#  define deflateParams Rio_deflateParams
#  define deflateBound  Rio_deflateBound
#  define inflateInit2_ Rio_inflateInit2_
#  define inflateSetDictionary Rio_inflateSetDictionary
#  define inflateSync   Rio_inflateSync
#  define inflateSyncPoint Rio_inflateSyncPoint
#  define inflateCopy   Rio_inflateCopy
#  define inflateReset  Rio_inflateReset
#  define compress      Rio_compress
#  define compress2     Rio_compress2
#  define compressBound Rio_compressBound
#  define uncompress    Rio_uncompress
#  define adler32       Rio_adler32
#  define crc32         Rio_crc32
#  define get_crc_table Rio_get_crc_table

#  define Byte          Rio_Byte
#  define uInt          Rio_uInt
#  define uLong         Rio_uLong
#  define Bytef         Rio_Bytef
#  define charf         Rio_charf
#  define intf          Rio_intf
#  define uIntf         Rio_uIntf
#  define uLongf        Rio_uLongf
#  define voidpf        Rio_voidpf
#  define voidp         Rio_voidp

/* Extras */
#define deflate_copyright  Rio_deflate_copyright 
#define deflate_fast Rio_deflate_fast
#define deflate_slow Rio_deflate_slow
#define deflate_stored Rio_deflate_stored 

#define inflateBack Rio_inflateBack
#define inflateBackEnd Rio_inflateBackEnd
#define inflateBackInit_ Rio_inflateBackInit_
#define inflate_fast Rio_inflate_fast
#define inflate_copyright Rio_inflate_copyright
#define inflate_table Rio_inflate_table

#define _dist_code Rio__dist_code
#define _length_code Rio__length_code
#define _tr_align Rio__tr_align
#define _tr_flush_block Rio__tr_flush_block
#define _tr_init Rio__tr_init
#define _tr_stored_block Rio__tr_stored_block

#define configuration_table Rio_configuration_table
#define fill_window Rio_fill_window
#define flush_pending Rio_flush_pending
#define lm_init Rio_lm_init
#define longest_match Rio_longest_match
#define longest_match_fast Rio_longest_match_fast
#define my_version Rio_my_version
#define putShortMSB Rio_putShortMSB
#define read_buf Rio_read_buf

#define distfix Rio_distfix
#define fixedtables Rio_fixedtables
#define lenfix Rio_lenfix
#define order Rio_order
#define syncsearch Rio_syncsearch
#define updatewindow Rio_updatewindow

#define _tr_tally Rio__tr_tally
#define base_dist Rio_base_dist
#define base_length Rio_base_length
#define bi_flush Rio_bi_flush
#define bi_reverse Rio_bi_reverse
#define bi_windup Rio_bi_windup
#define bl_order Rio_bl_order
#define build_bl_tree Rio_build_bl_tree
#define build_tree Rio_build_tree
#define compress_block Rio_compress_block
#define copy_block Rio_copy_block
#define extra_blbits Rio_extra_blbits
#define extra_dbits Rio_extra_dbits
#define extra_lbits Rio_extra_lbits
#define gen_bitlen Rio_gen_bitlen
#define gen_codes Rio_gen_codes
#define init_block Rio_init_block
#define pqdownheap Rio_pqdownheap
#define scan_tree Rio_scan_tree
#define send_all_trees Rio_send_all_trees
#define send_tree Rio_send_tree
#define set_data_type Rio_set_data_type
#define static_bl_desc Rio_static_bl_desc
#define static_d_desc Rio_static_d_desc
#define static_dtree Rio_static_dtree
#define static_l_desc Rio_static_l_desc
#define static_ltree Rio_static_ltree
#define tr_static_init Rio_tr_static_init

#define base_dist Rio_base_dist
#define base_length Rio_base_length
#define bi_buf Rio_bi_buf
#define bi_valid Rio_bi_valid
#define bl_count Rio_bl_count
#define bl_desc Rio_bl_desc
#define bl_order Rio_bl_order
#define bl_tree Rio_bl_tree
#define compressed_len Rio_compressed_len
#define configuration_table Rio_configuration_table
#define d_buf Rio_d_buf
#define d_desc Rio_d_desc
#define depth Rio_depth
#define dist_code Rio_dist_code
#define dyn_dtree Rio_dyn_dtree
#define dyn_ltree Rio_dyn_ltree
#define eofile Rio_eofile
#define error_flag Rio_error_flag
#define extra_blbits Rio_extra_blbits
#define extra_dbits Rio_extra_dbits
#define extra_lbits Rio_extra_lbits
#define flag_bit Rio_flag_bit
#define flag_buf Rio_flag_buf
#define flags Rio_flags
#define heap Rio_heap
#define heap_len Rio_heap_len
#define heap_max Rio_heap_max
#define in_buf Rio_in_buf
#define in_offset Rio_in_offset
#define in_size Rio_in_size
#define input_len Rio_input_len
#define ins_h Rio_ins_h
#define l_buf Rio_l_buf
#define l_desc Rio_l_desc
#define last_dist Rio_last_dist
#define last_flags Rio_last_flags
#define last_lit Rio_last_lit
#define length_code Rio_length_code
#define level Rio_level
#define lookahead Rio_lookahead
#define max_lazy_match Rio_max_lazy_match
#define opt_len Rio_opt_len
#define out_buf Rio_out_buf
#define out_offset Rio_out_offset
#define out_size Rio_out_size
#define sliding Rio_sliding
#define static_dtree Rio_static_dtree
#define static_len Rio_static_len
#define static_ltree Rio_static_ltree
#define verbose Rio_verbose
#define zfile Rio_zfile

#define bb Rio_bb
#define bk Rio_bk
#define border Rio_border
#define cpdext Rio_cpdext
#define cpdist Rio_cpdist
#define cplens Rio_cplens
#define cplext Rio_cplext
#define dbits Rio_dbits
#define hufts Rio_hufts
#define ibufcnt Rio_ibufcnt
#define ibufptr Rio_ibufptr
#define lbits Rio_lbits
#define mask Rio_mask
#define obufcnt Rio_obufcnt
#define obufptr Rio_obufptr
#define qflag Rio_qflag

#define crc32_big Rio_crc32_big
#define crc32_little Rio_crc32_little
#define crc_table Rio_crc_table

#define check_header Rio_check_header
#define destroy Rio_destroy
#define do_flush Rio_do_flush
#define getLong Rio_getLong
#define get_byte Rio_get_byte
#define gz_magic Rio_gz_magic
#define gz_open Rio_gz_open
#define gzclearerr Rio_gzclearerr
#define gzclose Rio_gzclose
#define gzdopen Rio_gzdopen
#define gzeof Rio_gzeof
#define gzerror Rio_gzerror
#define gzflush Rio_gzflush
#define gzgetc Rio_gzgetc
#define gzgets Rio_gzgets
#define gzopen Rio_gzopen
#define gzprintf Rio_gzprintf
#define gzputc Rio_gzputc
#define gzputs Rio_gzputs
#define gzread Rio_gzread
#define gzrewind Rio_gzrewind
#define gzseek Rio_gzseek
#define gzsetparams Rio_gzsetparams
#define gztell Rio_gztell
#define gzungetc Rio_gzungetc
#define gzwrite Rio_gzwrite
#define putLong Rio_putLong

#define fixedtables Rio_fixedtables
#define zError Rio_zError
#define zcalloc Rio_zcalloc
#define zcfree Rio_zcfree
#define zlibCompileFlags Rio_zlibCompileFlags
#define zlibVersion Rio_zlibVersion

#define dbase Rio_dbase
#define dext Rio_dext
#define lbase Rio_lbase
#define lext Rio_lext

#endif

