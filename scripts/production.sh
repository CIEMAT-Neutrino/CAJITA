#!/bin/bash

# time ./build/box1 VDGeo/UsualGeo_30cm_prod.json         data/UsualGeo_30cm.root
# time ./build/box1 VDGeo/UsualGeo_70cm_prod.json         data/UsualGeo_70cm.root
# time ./build/box1 VDGeo/UsualGeo_90cm_prod.json         data/UsualGeo_90cm.root
#son 10 mm en verda XD y 5mm
time ./build/box1 VDGeo/UsualGeo_90cm_1cm_prod.json     data/UsualGeo_90cm_1cm.root
# time ./build/box1 VDGeo/UsualGeo_100cm_prod.json        data/UsualGeo_100cm.root
time ./build/box1 VDGeo/UsualGeo_105cm_prod.json            data/UsualGeo_105cm.root
time ./build/box1 VDGeo/UsualGeo_105cm_50mm_prod.json       data/UsualGeo_105cm_50mm.root