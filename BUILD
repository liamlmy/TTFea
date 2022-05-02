cc_library(
    name = 'ttfea',
    srcs = [
        'factroy.cpp',
        'resc_factory.cpp',
        'resource.cpp',
        'extractor.cpp',
        'fea_base.cpp',
        'fea_common.cpp',
        'fea_utils.cpp',
        'parser.cpp',
    ],
    deps = [
        '//cplusutils/time:time',
        '//cplusutils/logutil:logutil',
        '//serv_base:serv_base'
    ],
    optimize = [
        'O2',
        'std=c++11',
    ]
    #prebuilt = True,
)
cc_binary(
    name = 'fea_extract',
    srcs = [
        'offline_runner.cpp',
    ],
    deps = [
        '//cplusutils/time:time',
        '//cplusutils/logutil:logutil',
        '//strategy_common/ttfea:ttfea',
        '//serv_base:serv_base'
    ],
    optimize = [
        'O2',
        'std=c++11',
        '-Wframe-larger-than=102400',
        '-Wno-float-equal',
    ]
)