AUTOMAKE_OPTIONS = foreign

includedir	 = ${prefix}/include/mtl

EXTRA_HEADERS  =		\
	abs.h			\
	array2D.h		\
	banded_indexer.h	\
	bare_bones_array.h	\
	blais.h			\
	blas_interface.cc	\
	blas_interface.h	\
	block1D.h		\
	block2D.h		\
	compressed1D.h		\
	compressed2D.h		\
	compressed_iter.h	\
	mtl_config.h.in		\
	conj.h			\
	cplx_dbldbl.h		\
	dense1D.h		\
	dense2D.h		\
	dense_iterator.h	\
	diagonal_indexer.h	\
	dimension.h		\
	dim_calc.h		\
	dumptofile.h		\
	elt.h			\
	entry.h			\
	envelope2D.h		\
	external_vector.h	\
	fast.h			\
	harwell_boeing_stream.h	\
	initialize.h		\
	iohb.c			\
	iohb.h			\
	iterator_adaptor.h	\
	lapack_interface.h	\
	light1D.h		\
	light_matrix.h		\
	linalg_vec.h		\
	matlabio.h		\
	matrix.h		\
	matrix_implementation.h	\
	matrix_market_stream.h	\
	matrix_stream.h		\
	matrix_traits.h		\
	meta_equal.h		\
	meta_if.h		\
	mmio.c			\
	mmio.h			\
	mtl.h			\
	mtl_limits.h		\
	mtl_set.h		\
	mtl_complex.h		\
	mtl_iterator.h		\
	lu.h                    \
	mtl2lapack.h		\
	mtl_algo.h		\
	mtl_exception.h		\
	norm.h			\
	not_at.h		\
	oned_part.h		\
	orien.h			\
	partition.h             \
	rect_indexer.h		\
	refcnt_ptr.h		\
	reverse_iter.h		\
	scale_iterator.h	\
	scaled1D.h		\
	scaled2D.h		\
	sparse1D.h		\
	sparse_iterator.h	\
	stamp-h.in		\
	strided1D.h		\
	strided_iterator.h	\
	transform_iterator.h	\
	uplo.h			\
	utils.h

EXTRA_DIST  =		$(EXTRA_HEADERS)

make-install-dirs:
	-if test '!' -d $(includedir); then mkdir -p $(includedir); fi

install-data-hook: make-install-dirs
	-@ echo Installing $(includedir) ; \
	$(INSTALL_DATA) *.h $(includedir)
