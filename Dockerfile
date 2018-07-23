FROM gcc:8.1

ENV BOOST_SOURCES_URL https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.bz2
ENV BOOST_DIR /boost

RUN mkdir ${BOOST_DIR}
RUN curl -L ${BOOST_SOURCES_URL} | tar xj -C ${BOOST_DIR} --strip-components=1
WORKDIR ${BOOST_DIR}
RUN ./bootstrap.sh --with-libraries=program_options
RUN ./b2 -j$(nproc) --link=static --runtime-link=static

ENV BOOST_INCLUDE_DIR ${BOOST_DIR}
ENV BOOST_LIB_DIR ${BOOST_DIR}/stage/lib

WORKDIR /app
