#!/usr/bin/env php
<?php

array_shift($argv);
$argv = array_values($argv);

if (!pcntl_fork()) {
	$argv[] = "esteh";
	$argv[] = "test.tea";
	pcntl_exec("/usr/bin/valgrind", $argv);
}
pcntl_wait($status);
