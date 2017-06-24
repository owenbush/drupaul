<?php

/**
 * @file
 * These are the hooks that are invoked by dRuPaul.
 */

/**
 * Alter the search terms defined by other modules.
 */
function hook_drupaul_search_terms_alter(&$search_terms) {
  // Show me The Hoff.
  $search_terms[] = 'David Hasselhoff';
}
