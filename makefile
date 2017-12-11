all:
	$(MAKE) -s -C aug_sum
	$(MAKE) -s -C interval
	$(MAKE) -s -C range_query
	$(MAKE) -s -C index

clean:
	$(MAKE) -s -C aug_sum clean
	$(MAKE) -s -C interval clean
	$(MAKE) -s -C range_query clean
	$(MAKE) -s -C index clean
