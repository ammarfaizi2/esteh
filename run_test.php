<?php

$esteh_bin = escapeshellarg(__DIR__."/esteh");


function esteh_test($file)
{
	printf("Testing \"%s\"...", $file);

	$handle = fopen($file, "r");
	
	// Read --TEST-- section.
	$r = trim(fgets($handle));

	if ($r !== "--TEST--") {
		printf("\tError: Couldn't find --TEST-- section in %s\n", $file);
		exit(1);
	}

	$desc = "";
	do {
		$r = trim(fgets($handle));
		if ($r === "--FILE--") {
			break;
		}
		$desc .= $r . "\n";

		if (feof($handle)) {
			printf("\tError: Couldn't find --FILE-- section in %s\n", $file);
			exit(1);
		}

	} while (true);
	$desc = trim($desc);

	$tmp_handle = fopen($compiled_file = "/tmp/tea_test_".basename($file), "w");
	do {
		$r = fgets($handle);
		if ($r === "--EXPECT--\n") {
			break;
		}
		fwrite($tmp_handle, $r);

		if (feof($handle)) {
			printf("\tError: Couldn't find --EXPECT-- section in %s\n", $file);
			fclose($tmp_handle);
			unlink($compiled_file);
			exit(1);
		}
	} while (1);
	fclose($tmp_handle);

	$r = "";
	do {
		$r .= fread($handle, 2048);
	} while (!feof($handle));


	global $esteh_bin;
	$exe = shell_exec("{$esteh_bin} --no-debug {$compiled_file} 2>/dev/null");
	if ($exe === $r) {
		printf("OK\n");
	} else {
		printf("\nError in \"%s\"!\n", $file);
		printf("\nExpected:\n%s\n", json_encode($r));
		printf("\nActual:\n%s\n", json_encode($exe));
		unlink($compiled_file);
		exit(1);
	}
	unlink($compiled_file);
}


// Basic test.
foreach (glob(__DIR__."/tests/basic/*.teat") as $file) {
	esteh_test($file);
}
