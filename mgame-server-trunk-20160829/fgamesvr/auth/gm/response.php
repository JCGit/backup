<?php

class Response {
    public static function JsonRender($data = array()) {
        die(json_encode($data));
    }

    public static function Json($data) {
        die($data);
    }
}