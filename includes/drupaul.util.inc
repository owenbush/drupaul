<?php

/**
 * @file
 * Utility functionality for the dRuPaul module.
 */

/**
 * Swap any mundane images with glamorous Ru Paul photos.
 *
 * @param array $build
 *    The entity build array.
 * @param string $field_name
 *    The field name to glamorize.
 */
function _drupaul_make_the_magic_happen(&$build, $field_name) {
  if (!empty($build[$field_name])) {
    foreach ($build[$field_name] as $key => $field) {
      if (!is_numeric($key)) {
        continue;
      }
      try {
        $uri = _drupaul_generate_random_image();
        if (!empty($uri)) {
          // Legally we have to give props to Giphy.
          $giphy_attribution = array(
            '#type' => 'container',
            'image' => array(
              '#theme' => 'image',
              '#path' => drupal_get_path('module', 'drupaul') . '/images/powered-by-giphy.png', 
              '#alt' => t('Powered By Giphy'),
              '#title' => t('Powered By Giphy'),
            ),
          );

          $build[$field_name][$key] = array(
            '#theme' => 'image',
            '#path' => $uri,
            '#width' => $build[$field_name][$key]['#item']['width'],
            '#height' => $build[$field_name][$key]['#item']['height'],
            '#title' => $build[$field_name][$key]['#item']['title'],
            '#alt' => $build[$field_name][$key]['#item']['alt'],
            '#suffix' => render($giphy_attribution),
          );
        }
      }
      catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
    }
  }
}

/**
 * Generate a random Ru Paul image.
 *
 * @return string
 *    The image uri.
 */
function _drupaul_generate_random_image() {
  $uri = '';

  $giphy_url = variable_get('drupaul_giphy_url');

  $params = array(
    'api_key' => variable_get('drupaul_api_key'),
    'tag' => _drupaul_get_search_terms(),
    'rating' => 'g'
  );

  // Add in the query params.
  $query = drupal_http_build_query($params);
  if (!empty($query)) {
    $giphy_url .= '?' . $query;
  }

  // Make the request, get some Ru Paul.
  $response = drupal_http_request($giphy_url);

  $response_data = json_decode($response->data);

  // Ru Paul says no, error code returned.
  if ($response->code != '200') {
    throw new Exception(t('Ru Paul Does Not Approve - Error @code: @message', array(
      '@code' => $response->code,
      '@message' => $response_data->message,
    )));
  }
  else {
    // If we actually got some Ru Paul then return it.
    if (!empty($response_data->data->image_url)) {
      $uri = $response_data->data->image_url;
    }
  }

  return $uri;
}

/**
 * Get a string of search terms for Giphy.
 * 
 * @return string
 *    A string of search terms to search for.
 */
function _drupaul_get_search_terms() {
  $terms = array(
    'ru paul',
  );

  // Allow other modules to decide what to search for.
  drupal_alter('drupaul_search_terms', $terms);
  $terms = implode(',', $terms);
  return $terms;
}
