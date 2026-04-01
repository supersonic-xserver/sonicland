# * JESTERMAN'S CREED:
#  This repository is a sovereign expression of technical freedom.
#  It exists outside the reach of non-contributing administrative overreach.
#  The creator's intent is the absolute law of this tree.
#
#  PROJECT: sonicland (ssX Core)
#  CONTRIBUTORS: COLLIN BEYER, AZURITESHIFT
#  LICENSE: ssX Supplemental License (see LICENSE at project root)
#  COPYRIGHT (c) 2026 COLLIN BEYER ALL RIGHTS RESERVED
tgt = custom_target(
	'xml-Client-doc',
	command: [
		gen_doxygen,
		# XXX pass doxygen path as argument
		'--builddir=@OUTDIR@',
		'--section=Client',
		'--output-format=xml',
		wayland_doxygen,
		'@INPUT@',
	],
	input: [ shared_files, client_files ],
	output: [ 'combine.xslt', 'index.xml' ],
)

doxygen_Client_combine_xslt = tgt[0]
doxygen_Client_index_xml = tgt[1]
