#!/usr/bin/python3
# coding=Big5

import sys 	#for argv
import re	#for reg

with open ( sys.argv[1], "r", encoding = 'big5hkscs') as Big5_ZhuYin:
	data = Big5_ZhuYin.read()

lines = data.split("\n")
#obtain list of lines

mapping = {}	
#creat dictionary

for line in lines[:-1]:
	elements = re.findall(r"[\w']+", line);
	ChineseChar = elements[0]

	for zhuyin in elements[1:]:
		#print(zhuyin)
		if zhuyin[0] not in mapping:
			mapping[zhuyin[0]] = set()

		mapping[zhuyin[0]].add(ChineseChar)
#done making dictionary
	
with open ( sys.argv[2], "w", encoding = 'big5hkscs') as ZhuYin_Big5:
	for zhuyin in mapping:
		print ("{} {}".format(zhuyin, " ".join(str(x) for x in mapping[zhuyin]) ), file=ZhuYin_Big5)
