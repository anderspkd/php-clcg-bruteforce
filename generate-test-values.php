<?php

if(count($argv) == 2) {
    $until = (int)$argv[1];
} else {
    $until = 10;
}

for($x = 0; $x < $until; $x++) {
    $uid = uniqid('', true);
    /* printf("%s %d\n", $uid, (1/4.656613e-10)*((float)substr($uid, -10) / 10));*/
    printf("%s\n", $uid);
}

?>
