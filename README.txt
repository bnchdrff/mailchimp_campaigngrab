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
diff --git a/modules/mailchimp_campaign/mailchimp_campaign.module b/modules/mailchimp_campaign/mailchimp_campaign
index e899b62..8f3d1a4 100644
--- a/modules/mailchimp_campaign/mailchimp_campaign.module
+++ b/modules/mailchimp_campaign/mailchimp_campaign.module
@@ -145,6 +145,11 @@ function mailchimp_campaign_overview_page() {
       l(t('View'), 'admin/config/services/mailchimp/campaigns/' . $campaign->mc_campaign_id),
     );
 
+    $context = array(
+      'campaign' => clone $campaign,
+    );
+    drupal_alter('mailchimp_campaign_actions', $actions, $context);
+
     $rows[] = array(
       l($campaign->label(),
         'admin/config/services/mailchimp/campaigns/' . $campaign->mc_campaign_id),
--8<--
