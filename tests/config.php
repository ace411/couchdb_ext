<?php

const CONFIG = ['localhost', '', '', 5984, 60];

$couch = new CouchDb(...CONFIG);