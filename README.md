CloudFlare is a FREE reverse proxy, firewall, and global content delivery network and can be implemented without installing any server software or hardware.

What does this module do?
-------------------------
1. Corrects $_SERVER["REMOTE_ADDR"] so it contains the IP address of your visitor, not CloudFlare's reverse proxy server.
2. Appropriately sets $_SERVER['HTTPS'] when CloudFlare's X-Forwarded-Proto header is detected.
3. Adds appropriate headers when serving the AlwaysOnline crawler
4. Integrates with CloudFlare's Threat API so you can ban and whitelist IP addresses from the Drupal Comment administration screen.
5. Integrates with CloudFlare's Spam API.
6. Integrates with CloudFlare’s Client Interface API (planned).


How do I get started with CloudFlare?
-------------------------------------
1. Visit http://www.cloudflare.com to sign up for a free account.
2. Follow their 5-minute configuration wizard.
3. Install this module and configure
   A. Install and Enable this module.
   B. Assign permissions to administer module (if you are not logged in as the user 1, admin)
   C. Save your email address and CloudFlare API key to the CloudFlare administration screen on your Drupal web site (admin/settings/cloudflare).
   D. Configure IP address detection as instructed below


IP Address detection
--------------------

Because your site will be proxied via CloudFlare, the global variable $_SERVER["REMOTE_ADDR"] will contain a CloudFlare IP address, not the client IP.  There are two ways this can be corrected. 


# Default Method: Use X-Forwarded-For Headers

The CloudFlare module will automatically modify the incoming $_SERVER["HTTP_X_FORWARDED_FOR"] header to remove CloudFlare IPs from the list of possible connecting IP addresses. This means that you can use Drupal's built-in X-Forwarded-For header handling and evertying will "just work". There is no special configuration required for this set-up. If you are using a reverse proxy such as varnish, you can configure your $conf['reverse_proxy_addresses'] array just as before and everything should work as expected.


# Alternative Method: Use CF-Connecting-IP header. 

CloudFlare provides a header called CF-Connecting-IP. We can use this header to detect the correct client IP address. When the cloudflare module is configured to use CF-Connecting-IP it will use this to set $_SERVER["REMOTE_ADDR"], and disable the processing of X-Forwarded-For headers. To use this set-up, simply place this code somewhere in your settings.php file:

```
$conf['cloudflare_cf_connecting_ip'] = TRUE;
```


Using a reverse proxy
---------------------

If your site is behind a reverse-proxy such as varnish, you should configure your outer reverse proxy to strip incoming cloudflare specific headers (CF-Connecting-IP, CF-Railgun, CF-Ray, CF-Visitor, and CF-IPCountry) if the incoming request is not from a valid CloudFlare IP address (https://www.cloudflare.com/ips). This ensures that bad actors cannot spoof CloudFlare headers and impersonate a different connecting IP address or other information.


Always Online
-------------

To use CloudFlare's AlwaysOnline feature, you will need to be using a reverse proxy such as varnish. To trigger always-online, you'll need to serve a 520 in place of 500 and 503 errors. To do this, put the following in your varnish config file:

```
sub vcl_deliver {
  # If proxying via cloudflare, then send 520 responses in place of 500/503
  if ((resp.status == 500 || resp.status == 503) && req.http.cf-connecting-ip) {
    set resp.status = 520;
  }
}
```

Agressive Caching
-----------------

CloudFlare allows aggressive caching of HTML pages via custom page-rules. If you enable this option in your CloudFlare settings CloudFlare will serve the entire page out of cache, omitting a call to Drupal entirely. This can be problematic if logged-in users (or admin users) see the page differently than regular users. An example of this would be menus that only logged in users can see. In this case it's quite possible for CloudFlare to aggressively cache the "logged-in user" view of the page and serve everyone this view!   

There are two ways to fix this:

1. Set up a different domain name for administrative users. For example, you would have both admin.example.com and www.example.com pointing to the same drupal site, and you would instruct your admin users to use admin.example.com.  In your cloudflare settings you would turn off cloudflare for admin.example.com but keep it enabled for the regular www.example.com. 

2. Disable aggressive page caching. If you *must* allow users to login via the same domain as anonymous users, then aggressive caching likely isn’t for you unless you have the skill to implement AJAX based menus and user-dependent content. Generally in this situation aggressive page caching shouldn't be used.
