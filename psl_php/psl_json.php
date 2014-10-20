<?php

header("Content-Type: application/json", true);

include 'tmhOAuth/tmhOAuth.php';

$tmhOAuth = new tmhOAuth(array(
  'consumer_key' => 'CONSUMERKEY',
  'consumer_secret' => 'CONSUMERSECRET',
  'token' => 'YOURTOKEN',
  'secret' => 'YOURSECRET',
));

$tmhOAuth->request(
  'GET',
  'https://api.twitter.com/1.1/search/tweets.json',
   array(
       'q' => 'pumpkin spice latte',
       'include_entities'=>false,
       'count' => '1',
       'result_type' => 'recent'
   )
);

$response_data = json_decode($tmhOAuth -> response['response'], true);

$arr = array('id' => $response_data['statuses'][0]['id_str'], 'timestamp' => $response_data['statuses'][0]['created_at'], 'tweet' => $response_data['statuses'][0]['text'], 'user' => $response_data['statuses'][0]['user']['screen_name']);

$id = $response_data['statuses'][0]['id_str'];
$tweetURL = "http://psl.local/mailbox/" . $id;

$ch = curl_init();
curl_setopt($ch,CURLOPT_URL, $tweetURL);
curl_setopt($ch,CURLOPT_HEADER,0);
curl_exec($ch);
curl_close($ch);

echo json_encode($arr);
?>