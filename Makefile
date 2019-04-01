.PHONY: run

MD2REVIEW_DOCKER_IMG=nuitsjp/md2review:1.12.0
REVIEW_DOCKER_IMG=vvakame/review:3.0
WORK_DIR=/work
SRC_DIR=`pwd`/src

review:
	docker run --rm -w=$(WORK_DIR) -v $(SRC_DIR):$(WORK_DIR) $(MD2REVIEW_DOCKER_IMG) /bin/sh -c "md2reviews.sh"

pdf: review
	docker run --rm -w=$(WORK_DIR) -v $(SRC_DIR):$(WORK_DIR) $(REVIEW_DOCKER_IMG) /bin/sh -c "review-pdfmaker config.yml" 

epub: review
	docker run --rm -w=$(WORK_DIR) -v $(SRC_DIR):$(WORK_DIR) $(REVIEW_DOCKER_IMG) /bin/sh -c "review-epubmaker config.yml" 
	
pull:
	docker pull $(MD2REVIEW_DOCKER_IMG)
	docker pull $(REVIEW_DOCKER_IMG)
