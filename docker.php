<?php


$dir = __DIR__;
$handle = proc_open(
	"exec docker run --rm --memory 10485760 -v {$dir}:/root/esteh -w /root/esteh ub0 /root/esteh/esteh test.tea",
	[
		["pipe", "r"],
		["file", "php://stdout", "w"],
		["file", "php://stderr", "w"],
	],
	$pipes
);

proc_close($handle);
