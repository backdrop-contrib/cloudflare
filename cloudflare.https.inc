<?php

// To properly handle flexible SSL handling, call hook_boot before sessions start
global $cloudflare_ssl_handling;
$cloudflare_ssl_handling = TRUE;
require_once("cloudflare.module");
cloudflare_boot();
