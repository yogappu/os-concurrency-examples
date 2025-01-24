#!/bin/bash
for i in {0..10}
do
    sleep 1&
done
exec sleep 100