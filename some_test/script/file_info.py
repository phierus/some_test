#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import subprocess
import fnmatch
import json


def GetFilesList(file_list, path):
	if os.path.exists(path):
		files = os.listdir(path)
		for file in files:
			file_path = os.path.join(path, file)
			if os.path.isfile(file_path):
				if fnmatch.fnmatch(file_path, '*.mp3'):
					file_list.append(file_path)
			else:
				GetFilesList(file_list, file_path)

def GetFileJsonString(file_path):
	cmd = '"%s\\%s" -v quiet -print_format json -show_format -show_streams -i "%s"' %(
		os.path.abspath('.'), 'ffprobe.exe', file_path)

	ps = subprocess.Popen(cmd, stdout = subprocess.PIPE)
	ps.wait()

	out_str = ps.stdout.read()
	return out_str


if __name__ == '__main__': 

	file_path_list = []
	GetFilesList(file_path_list, os.path.join(os.path.abspath('.'), 'out'))

	total_duration = 0

	group_edge = 65
	group_duration = 0
	group_index = 1
	print '='*15, '   ', group_index, '   ', '='*15
	group_index += 1

	file_index = 1
	group_start_index = file_index
	for f in file_path_list:
		if 0:
			print f
			file_index += 1
		else:
			try:
				js = json.loads(GetFileJsonString(f))

				duration = int(float(js['format']['duration']))
				total_duration += duration

				group_duration += duration
				if group_duration/60 > group_edge:
					print  '>>>>> file: ', group_start_index, '--->', file_index-1
					# print '='*15, '   ', group_index, '   ', '='*15
					group_duration = 0
					group_index += 1
					group_start_index = file_index

				# print file_index, duration/60, "::", duration%60
				file_index += 1
			except Exception, e:
				print Exception, ": ", e, f

	print  '>>>>> file: ', group_start_index, '--->', file_index-1
	print '='*20, '   grop_edget:', group_edge, '   ', '='*20
	print total_duration, "=> ", total_duration/60, ":", total_duration%60
