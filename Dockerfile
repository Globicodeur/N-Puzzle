FROM gcc:5.3

ENV BOOST_SOURCES_URL http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.bz2/download
ENV BOOST_DIR /boost

RUN mkdir ${BOOST_DIR}
RUN curl -L ${BOOST_SOURCES_URL} | tar xj -C ${BOOST_DIR} --strip-components=1
WORKDIR ${BOOST_DIR}
RUN ./bootstrap.sh --with-libraries=program_options
RUN ./b2 -j4 --link=static --runtime-link=static

ENV BOOST_INCLUDE_DIR ${BOOST_DIR}
ENV BOOST_LIB_DIR ${BOOST_DIR}/stage/lib

WORKDIR /app
