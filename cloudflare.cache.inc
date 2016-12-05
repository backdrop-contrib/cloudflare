<?php

include_once("./cloudflare.module");

// To properly handle flexible SSL handling, call hook_boot before sessions start
global $cloudflare_ssl_handling;
$cloudflare_ssl_handling = TRUE;
cloudflare_boot();
