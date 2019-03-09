.PHONY: run

MD2REVIEW_DOCKER_IMG=nuitsjp/md2review:1.12.0
REVIEW_DOCKER_IMG=vvakame/review:3.1
WORK_DIR=/work

review:
	docker run --rm -w=$(WORK_DIR) -v `pwd`/src:$(WORK_DIR) $(MD2REVIEW_DOCKER_IMG) /bin/sh -c "md2reviews.sh"

pdf: review
	docker run --rm -w=$(WORK_DIR) -v `pwd`/src:$(WORK_DIR) $(REVIEW_DOCKER_IMG) /bin/sh -c "review-pdfmaker config.yml" 
	
pull:
	docker pull $(MD2REVIEW_DOCKER_IMG)
	docker pull $(REVIEW_DOCKER_IMG)
