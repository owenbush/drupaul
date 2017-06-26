<?php

/**
 * @file
 * DRuPaul Productivity Tool Admin Functionality.
 */

/**
 * Page Callback: drupaul_admin_settings.
 */
function drupaul_admin_settings() {
  $entity_types = field_info_bundles();

  $entities = $bundles = array();
  if (!empty($entity_types)) {
    $entities = array_keys($entity_types);
    foreach ($entity_types as $entity_type => $bundle) {
      $bundles = array_merge($bundles, array_keys($bundle));
    }
  }

  $key = 'drupaul_chosen_entities';
  $form[$key] = array(
    '#type' => 'checkboxes',
    '#title' => t('dRuPauled Entities'),
    '#description' => t('Select which entities should get the dRuPaul treatment.'),
    '#options' => drupal_map_assoc($entities),
    '#default_value' => variable_get($key, ''),
  );

  $key = 'drupaul_chosen_bundles';
  $form[$key] = array(
    '#type' => 'checkboxes',
    '#title' => t('dRuPauled Bundles'),
    '#description' => t('Select which bundles should get the dRuPaul treatment.'),
    '#options' => drupal_map_assoc($bundles),
    '#default_value' => variable_get($key, ''),
  );

  return system_settings_form($form);

}
