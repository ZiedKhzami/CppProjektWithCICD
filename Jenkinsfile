pipline
{
    agent none

    stages
    {
        stage('Build & Test (Windows + Linux Matrix)')
        {
            matrix
            {
                axes
                {
                    axis { name 'OS';           values 'windows', 'linux' }
                    axis { name 'BUILD_TYPE';   values 'Debug', 'Release' }
                    axis { name 'compiler';     values 'msvc', 'gcc', 'clang' }
                }
                excludes
                {
                    exclude { axis {name 'OS'; value 'windows'} axis {name 'compiler'; value 'gcc' }}
                    exclude { axis {name 'OS'; value 'windows'} axis {name 'compiler'; value 'clang' }}
                    exclude { axis {name 'OS'; value 'linux'} axis {name 'compiler'; value 'msvc'}}
                }

                agent { label "${OS == 'windows' ? 'windows-msvc' : 'linux' }"}

                stages
                {
                    stage('Toolchain Info')
                    {
                        steps
                        {
                            script
                            {
                                if(env.OS == 'windows')
                                {
                                    bat '''
                                    echo "=== System Information ==="
                                    uname -a || true

                                    echo "=== Compiler Versions ==="
                                    call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                    cl /Bv

                                    echo "=== Build Tools ==="
                                    cmake --version
                                    ninja --version
                                    '''
                                }
                                else
                                {
                                    sh '''
                                    echo "=== System Information ==="
                                    uname -a || true

                                    echo "=== Compiler Versions ==="
                                    gcc --version || true
                                    g++ --version || true
                                    clang --version || true

                                    echo "=== Build Tools ==="
                                    cmake --version
                                    ninja --version    
                                    '''
                                } // if-else
                            } // script
                        } // steps
                    } //stage 

                    stage('configure')
                    {
                        steps
                        {
                            script
                            {
                                String cfg_preset = ''
                                if(env.OS == 'Windows')
                                {
                                    cfg_preset = (env.BUILD_TYPE == 'Debug') ? 'windows-debug' : 'windows-release'
                                }
                                else if(env.COMPILER == "gcc")
                                {
                                    cfg_preset = (env.BUILD_TYPE == 'Debug') ? 'linux-gcc-debug' : 'linux-gcc-release'
                                }
                                else if(env.COMPILER == "clang")
                                {
                                    cfg_preset = (env.BUILD_TYPE == 'Debug') ? 'linux-clang-debug' : 'linux-clang-release'
                                }

                                if(env.OS == 'Windows')
                                {
                                    bat '''
                                    call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                    cmake --preset ${cfgPreset}
                                    '''
                                }
                                else
                                {
                                    sh "cmake --preset ${cfgPreset}"
                                }
                            } // script
                        } // steps
                    }// stage    

                    stage('Build')
                    {
                        steps {
                            script {
                                String buildPreset = ''
                                if (env.OS == 'windows') {
                                buildPreset = (env.BUILD_TYPE == 'Debug') ? 'build-debug' : 'build-release'
                                } else if (env.COMPILER == 'gcc') {
                                buildPreset = (env.BUILD_TYPE == 'Debug') ? 'gcc-debug' : 'gcc-release'
                                } else {
                                buildPreset = (env.BUILD_TYPE == 'Debug') ? 'clang-debug' : 'clang-release'
                                }

                                if (env.OS == 'windows') {
                                bat """
                                    call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                    cmake --build --preset ${buildPreset} --verbose
                                """
                                } else {
                                sh "cmake --build --preset ${buildPreset} --verbose"
                                }
                            }
                        }
                    }

                    stage('Format Check (Debug only)')
                    {
                        when { expression { env.BUILD_TYPE == 'Debug' } }
                        steps
                        {
                            script
                            {
                                if (env.OS == 'Windows')
                                {
                                    bat """
                                    call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                    cmake --build build\\windows-debug --target format-check
                                    """
                                }
                                else
                                {
                                    String dir = (env.COMPILER == 'gcc') ? 'build/linux-gcc-debug' : 'build/linux-clang-debug'
                                    sh "cmake --build ${dir} --target format-check"
                                }
                            }
                        }        
                    }

                    stage('clang-tidy (Debug only)')
                    {
                        when{expression { env.BUILD_TYPE == 'Debug'} }
                        steps
                        {
                            script
                            {
                                if (env.os == 'Windows')
                                {
                                    bat """
                                    call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                    cmake --build build\\windows-debug --target core_lib
                                    cmake --build build\\windows-debug --target app
                                    """
                                }
                                else 
                                {
                                    String dir = (env.COMPILER == 'gcc') ? 'build/linux-gcc-debug' : 'build/linux-clang-debug'
                                    sh """
                                        cmake --build ${dir} --target core_lib
                                        cmake --build ${dir} --target app
                                    """
                                } 
                            }
                        }
                    }

                    stage('Tests (Debug only)') 
                    {
                        when { expression { env.BUILD_TYPE == 'Debug' } }
                        steps {
                            script {
                                if (env.OS == 'windows') {
                                bat """
                                    call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                    ctest --preset test-debug --output-on-failure
                                """
                                } else {
                                if (env.COMPILER == 'gcc') {
                                    sh "ctest --test-dir build/linux-gcc-debug --output-on-failure"
                                } else {
                                    sh "ctest --test-dir build/linux-clang-debug --output-on-failure"
                                }
                                }
                            }
                        }
                    }

                    stage('Benchmarks (Release only)') 
                    {
                        when { expression { env.BUILD_TYPE == 'Release' } }
                        steps {
                            script {
                                if (env.OS == 'windows') 
                                {
                                    bat """
                                        call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                        cmake --build --preset build-release --target benchmarks
                                    """
                                } 
                                else 
                                {
                                    String buildPreset = (env.COMPILER == 'gcc') ? 'gcc-release' : 'clang-release'
                                    sh "cmake --build --preset ${buildPreset} --target benchmarks"
                                }
                            }
                        }
                    }

                    stage('Docs (optional)') 
                    {
                        when { expression { fileExists('docs/Doxyfile.in') && env.BUILD_TYPE == 'Debug' } }
                        steps {
                            script {
                                if (env.OS == 'windows') 
                                {
                                    bat """
                                        call "C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Auxiliary\\Build\\vcvars64.bat"
                                        cmake --build build\\windows-debug --target docs
                                    """
                                } 
                                else 
                                {
                                    String dir = (env.COMPILER == 'gcc') ? 'build/linux-gcc-debug' : 'build/linux-clang-debug'
                                    sh "cmake --build ${dir} --target docs"
                                }
                            }
                        }
                    }

                } // stages

                post
                {
                    always
                    {
                        script
                        {
                            if(env.BUILD_TYPE == 'Release')
                            {
                                if(env.OS == 'windows')
                                {
                                    archiveArtifacts artifacts: 'build/windows-release/bin/**', fingerprint: true, allowEmptyArchive: true
                                }
                                else
                                {
                                    String base = (env.COMPILER == 'gcc') ? 'build/linux-gcc-release' : 'build/linux-clang-release'
                                    archiveArtifacts artifacts: "${base}/bin/**", fingerprint: true, allowEmptyArchive: true
                                }
                            }
                        }
                    }
                }

            } // Matrix
        } // stage
    } // stages


    options
    {
        ansiColor('xterm')
        disableConcurrentBuilds()
        timestamps()
    }
} // pipline