#!/bin/bash

perl separator_big5.pl testdata/1.txt > testdata/1_perled.txt
disambig -text testdata/1_perled.txt -map ZhuYin-Big5.map -lm lanmodel.lm -order 2 -keep-unk > 1_ans.txt
./mydisambig testdata/1_perled.txt ZhuYin-Big5.map lanmodel 1_out.txt


perl separator_big5.pl testdata/2.txt > testdata/2_perled.txt
disambig -text testdata/2_perled.txt -map ZhuYin-Big5.map -lm lanmodel.lm -order 2 -keep-unk > 2_ans.txt
./mydisambig testdata/2_perled.txt ZhuYin-Big5.map lanmodel 2_out.txt


perl separator_big5.pl testdata/3.txt > testdata/3_perled.txt
disambig -text testdata/3_perled.txt -map ZhuYin-Big5.map -lm lanmodel.lm -order 2 -keep-unk > 3_ans.txt
./mydisambig testdata/3_perled.txt ZhuYin-Big5.map lanmodel 3_out.txt


perl separator_big5.pl testdata/4.txt > testdata/4_perled.txt
disambig -text testdata/4_perled.txt -map ZhuYin-Big5.map -lm lanmodel.lm -order 2 -keep-unk > 4_ans.txt
./mydisambig testdata/4_perled.txt ZhuYin-Big5.map lanmodel 4_out.txt


perl separator_big5.pl testdata/5.txt > testdata/5_perled.txt
disambig -text testdata/5_perled.txt -map ZhuYin-Big5.map -lm lanmodel.lm -order 2 -keep-unk > 5_ans.txt
./mydisambig testdata/5_perled.txt ZhuYin-Big5.map lanmodel 5_out.txt

