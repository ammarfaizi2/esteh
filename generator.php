<?php

/**
 * @param string $dir
 * @param \Closure $callback
 * @return void
 */
function rscan_callback(string $dir, Closure $callback)
{
	$dir = realpath($dir);
	$scan = scandir($dir);
	unset($scan[0], $scan[1]);
	foreach ($scan as $k => $file) {
		if (is_dir($dir."/".$file)) {
			rscan_callback($dir."/".$file, $callback);
		} else {
			$callback($dir."/".$file);
		}
	}
}
	
/**
 * @param string $str
 * @return string
 */
function iescape(string $str): string
{
	return str_replace(
        ["\\", "\""],
        ["\\\\", "\\\""],
        $str
	);
}
	
/**
 * @param string $str
 * @return string
 */
function rd(string $str): string
{
	return "\"".iescape($str)."\"";
}

$noCompile = true;
$closure = function (string $file) use ($argv, &$noCompile) {
	if (preg_match("/(.+)(?:\.frag\.)(h|c|cc|cpp)$/", $file, $f)) {

		$targetFile = $f[1].".".$f[2];
		$dontCompile = file_exists($targetFile) && (filemtime($file) < filemtime($targetFile));

		if (isset($argv[1])) {
			$noCompile = false;
			if ($argv[1] === "deps") {
				print $targetFile."\n";
				return;
			} else if ($argv[1] === "build") {
				print $file."\n";
				return;
			} else if ($argv[1] === "clean") {
				unlink($targetFile);
			}
		}

		if ($dontCompile) {
			return;
		}

		$noCompile = false;

		print "Compiling {$targetFile}...\n";
		$content = file_get_contents($file);
		ob_start();
		eval("?>".$content);
		file_put_contents($targetFile, ob_get_clean());
	}
};

rscan_callback(__DIR__."/src", $closure);
rscan_callback(__DIR__."/include", $closure);

if ($noCompile) {
	fprintf(STDERR, "generator.php: Nothing to be done for 'all'\n");
	exit(0);
}
