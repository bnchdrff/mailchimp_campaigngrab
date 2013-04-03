Mailchimp Campaign Grab
=======================

After configuring your Drupal site's webhook endpoint in MailChimp for certain lists, create Campaign entities in Drupal when they're sent.

Requires a patch to mailchimp.module:

--8<--

diff --git a/mailchimp.module b/mailchimp.module
index 9452acb..4e60d9c 100644
--- a/mailchimp.module
+++ b/mailchimp.module
@@ -389,6 +389,9 @@ function mailchimp_process_webhook() {
       break;
   }
 
+  // let modules do things
+  module_invoke_all('mailchimp_webhook', $_POST['type'], $data);
+
   // log event
   watchdog('mailchimp', 'Webhook type @type has been processed.',
     array('@type' => $_POST['type']), WATCHDOG_INFO

--8<--
