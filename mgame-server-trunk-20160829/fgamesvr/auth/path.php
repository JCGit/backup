 <?php
  require(dirname(__FILE__) . "/utils/util.php");
  $uri = $_REQUEST['param'];                                                                                                                                                                                                                                                  
  error_log(FormatLogStr('61game', 'https proxy', ERROR_0, 'uri :' . $uri));
  return "not found proxy https for uri: $uri";