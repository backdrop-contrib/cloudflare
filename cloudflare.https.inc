<?php
/**
 * @file
 * To handle flexible SSL handling by calling hook_boot() before sessions start.
 */

global $_cloudflare_ssl_handling;
$_cloudflare_ssl_handling = TRUE;
require_once "cloudflare.module";
cloudflare_boot();
