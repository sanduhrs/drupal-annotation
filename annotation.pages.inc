<?php

/**
 * @file
 * Annotation pages.
 */

/**
 * View annotation; Menu callback.
 */
function annotation_view_page($entity, $view_mode = 'full', $langcode = NULL) {
  if (isset($entity->id)) {
    return annotation_view($entity, $view_mode = 'full', $langcode = NULL);
  }
  else {
    return drupal_not_found();
  }
}

/**
 * Annotation view.
 */
function annotation_view($entity, $view_mode = 'full', $langcode = NULL) {
  $wrapper = entity_metadata_wrapper('annotation', $entity);
  return $wrapper->view();
}

/**
 * Delete form.
 */
function annotation_delete_form($form, &$form_state, $annotation) {
  $form_state['annotation'] = $annotation;

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $annotation->id,
  );

  $description = t('This action cannot be undone.');

  return confirm_form($form,
    t('Are you sure you want to delete the annotation %id?', array(
      '%id' => $annotation->id,
    )),
    'annotation/' . $annotation->id,
    $description,
    t('Delete')
  );
}

/**
 * Delete form; submit handler.
 */
function annotation_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm'] && $annotation = entity_object_load($form_state['values']['id'], 'annotation')) {
    entity_delete('annotation', $annotation->id);

    $args = array(
      '@type' => $annotation->type,
      '%id' => $annotation->id,
    );
    watchdog('annotation', '@type: deleted %id.', $args);
    drupal_set_message(t('Annotation %id has been deleted.', $args));
  }

  $form_state['redirect'] = '<front>';

  // Clear the page and block caches.
  cache_clear_all();
}
