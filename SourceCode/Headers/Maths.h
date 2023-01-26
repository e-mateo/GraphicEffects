#pragma once
#include <cmath>


// Vertex format (only one supported for now)
struct rdrVertex
{
    float x, y, z;    // Pos
    float nx, ny, nz; // Normal
    float r, g, b;    // Color
    float u, v;       // Texture coordinates
};

// Constant and common maths functions
namespace maths
{
    const float TAU = 6.283185307179586476925f;
    const float PI = 3.14159265358979323846f;

    inline float cos(float x) { return cosf(x); }
    inline float sin(float x) { return sinf(x); }
    inline float tan(float x) { return tanf(x); }
    inline float rad(float x) { return (x * PI / 180); }

    template <typename T>
    inline T min(T x, T y)
    {
        if (x <= y)
            return x;
        else
            return y;
    }

    template <typename T>
    inline T max(T x, T y)
    {
        if (x >= y)
            return x;
        else
            return y;
    }

    template <typename T>
    inline T Abs(T x)
    {
        if (x >= 0)
            return x;
        else
            return -x;
    }

    template <typename T>
    inline T Sign(T x)
    {
        if (x >= 0)
            return 1;
        else
            return -1;
    }

    template <typename T>
    inline T Min(T x, T y, T z)
    {
        if (x < y && x < z)
            return x;
        else if (y < x && y < z)
            return y;
        else
            return z;
    }

    template <typename T>
    inline T Min(T x, T y)
    {
        if (x < y)
            return x;
        else
            return y;
    }

    inline float lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    class Vec2
    {
    public:
        float x, y;

        Vec2()
        {
            x = 0;
            y = 0;
        }

        Vec2(const float vecX, const float vecY)
        {
            x = vecX;
            y = vecY;
        }

        inline float GetMagnitude() const
        {
            return (sqrtf(x * x + y * y));
        }

        inline void Normalize()
        {
            float magnitude = GetMagnitude();
            x = x / magnitude;
            y = y / magnitude;
        }
    };

    inline Vec2 operator+(const Vec2 Vector1, const Vec2 Vector2)
    {
        Vec2 newVec2(Vector1.x + Vector2.x, Vector1.y + Vector2.y);
        return newVec2;
    }

    inline Vec2 operator-(const Vec2 Vector1, const Vec2 Vector2)
    {
        Vec2 newVec2(Vector1.x - Vector2.x, Vector1.y - Vector2.y);
        return newVec2;
    }

    inline Vec2 operator*(const Vec2 Vector1, const float a)
    {
        Vec2 newVec2(Vector1.x * a, Vector1.y * a);
        return newVec2;
    }

    inline float dotProduct(const Vec2 vec1, const Vec2 vec2)
    {
        return (vec1.x * vec2.x + vec1.y + vec2.y);
    }

    inline float crossProduct(const Vec2 vec1, const Vec2 vec2)
    {
        return (vec1.x * vec2.y - vec2.x + vec1.y);
    }

    inline bool isInsideTriangle(Vec2 point, Vec2 vertex0, Vec2 vertex1, Vec2 vertex2)
    {
        float edge_0_1 = (point.x - vertex0.x) * (vertex1.y - vertex0.y) - (point.y - vertex0.y) * (vertex1.x - vertex0.x);
        float edge_1_2 = (point.x - vertex1.x) * (vertex2.y - vertex1.y) - (point.y - vertex1.y) * (vertex2.x - vertex1.x);
        float edge_2_0 = (point.x - vertex2.x) * (vertex0.y - vertex2.y) - (point.y - vertex2.y) * (vertex0.x - vertex2.x);

        if (edge_0_1 >= -0.02 && edge_1_2 >= -0.02 && edge_2_0 >= -0.02)
            return true;
        else
            return false;
    }

    inline float distanceBetweenTwoPoints(const Vec2 A, const Vec2 B)
    {
        float AB = (B - A).GetMagnitude();
        return AB;
    }

    inline Vec2 triangleGravityCenter(Vec2 vec1, Vec2 vec2, Vec2 vec3)
    {
        Vec2 gCenter;
        gCenter.x = (vec1.x + vec2.x + vec3.x) / 3;
        gCenter.y = (vec1.y + vec2.y + vec3.y) / 3;
        return gCenter;
    }

    inline float triangleArea(Vec2 vecA, Vec2 vecB, Vec2 vecC)
    {
        float a = distanceBetweenTwoPoints(vecA, vecB);
        float b = distanceBetweenTwoPoints(vecB, vecC);
        float c = distanceBetweenTwoPoints(vecC, vecA);
        float result = 0.25f * (sqrtf((a + b + c) * (-a + b + c) * (a - b + c) * (a + b - c)));
        return result;
    }

    ////////////////////////////////////////////////////////////////////////

    class Vec3
    {
    public:
        float x, y, z;

        Vec3()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        Vec3(const float vecX, const float vecY, const float vecZ)
        {
            x = vecX;
            y = vecY;
            z = vecZ;
        }

        inline float GetMagnitude() const
        {
            return (sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2)));
        }

        inline Vec3 Normalize()
        {
            float magnitude = GetMagnitude();
            Vec3 newVec;
            newVec.x = x / magnitude;
            newVec.y = y / magnitude;
            newVec.z = z / magnitude;
            return newVec;
        }

        inline Vec3 operator=(Vec3 other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }
    };

    inline Vec3 operator+(Vec3 Vector1, Vec3 Vector2)
    {
        Vec3 newVec3(Vector1.x + Vector2.x, Vector1.y + Vector2.y, Vector1.z + Vector2.z);
        return newVec3;
    }

    inline Vec3 operator-(Vec3 Vector1, Vec3 Vector2)
    {
        Vec3 newVec3(Vector1.x - Vector2.x, Vector1.y - Vector2.y, Vector1.z - Vector2.z);
        return newVec3;
    }

    inline Vec3 operator*(Vec3 Vector1, float a)
    {
        Vec3 newVec3(Vector1.x * a, Vector1.y * a, Vector1.z * a);
        return newVec3;
    }

    inline Vec3 operator*(float a, Vec3 Vector1)
    {
        Vec3 newVec3(Vector1.x * a, Vector1.y * a, Vector1.z * a);
        return newVec3;
    }

    inline bool operator==(Vec3 Vector1, Vec3 Vector2)
    {
        return(Vector1.x == Vector2.x && Vector1.y == Vector2.y && Vector1.z == Vector2.z);
    }

    inline Vec3 vectorProduct(const Vec3 vec1, const Vec3 vec2)
    {
        Vec3 newVector;
        newVector.x = vec1.y * vec2.z - vec1.z * vec2.y;
        newVector.y = vec1.z * vec2.x - vec1.x * vec2.z;
        newVector.x = vec1.x * vec2.y - vec1.y * vec2.x;
        return newVector;
    }

    inline float dotProduct(const Vec3 vec1, const Vec3 vec2)
    {
        return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
    }

    inline Vec3 findBarycentricCoor(Vec2 point, Vec2 vertexA, Vec2 vertexB, Vec2 vertexC)
    {
        float wA = triangleArea(point, vertexB, vertexC) / triangleArea(vertexA, vertexB, vertexC);
        float wB = triangleArea(point, vertexC, vertexA) / triangleArea(vertexA, vertexB, vertexC);
        float wC = triangleArea(point, vertexA, vertexB) / triangleArea(vertexA, vertexB, vertexC);
        Vec3 baryCoor(wA, wB, wC);
        return baryCoor;
    }

    inline Vec3 operator/(Vec3 Vector1, int a)
    {
        Vec3 newVec3(Vector1.x / a, Vector1.y / a, Vector1.z / a);
        return newVec3;
    }

    inline float distanceBetweenTwoPoints(const Vec3 A, const Vec3 B)
    {
        float AB = (B - A).GetMagnitude();
        return AB;
    }

    ////////////////////////////////////////////////////////////////////////

    class Vec4
    {
    public:
        float x, y, z, w;

        Vec4()
        {
            x = 0;
            y = 0;
            z = 0;
            w = 0;
        }

        Vec4(const float vecX, const float vecY, const float vecZ, const float vecW)
        {
            x = vecX;
            y = vecY;
            z = vecZ;
            w = vecW;
        }

        inline Vec3 Homogenize()
        {
            Vec3 newVec;
            if (w != 0)
            {
                newVec.x = x / w;
                newVec.y = y / w;
                newVec.z = z / w;
            }
            return newVec;
        }

        inline float GetMagnitude() const
        {
            return (sqrtf(x * x + y * y + z * z + w * w)); // x * x to not use powf
        }

        inline Vec3 NormalizeVec3()
        {
            Vec3 newVec;
            float magnitude = GetMagnitude();
            newVec.x = x / magnitude;
            newVec.y = y / magnitude;
            newVec.z = z / magnitude;
            return newVec;
        }

        inline Vec4 Normalize()
        {
            float magnitude = GetMagnitude();
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            w /= magnitude;
            return *this;
        }

        Vec4(const Vec3& vec3, float _w = 1.f)
        {
            x = vec3.x;
            y = vec3.y;
            z = vec3.z;
            w = 1.f;
        }
    };

    inline Vec4 operator+(Vec4 Vector1, Vec4 Vector2)
    {
        Vec4 newVec4(Vector1.x + Vector2.x, Vector1.y + Vector2.y, Vector1.z + Vector2.z, Vector1.w + Vector2.w);
        return newVec4;
    }

    inline Vec4 operator-(Vec4 Vector1, Vec4 Vector2)
    {
        Vec4 newVec4(Vector1.x - Vector2.x, Vector1.y - Vector2.y, Vector1.z - Vector2.z, Vector1.w - Vector2.w);
        return newVec4;
    }

    inline Vec4 operator*(Vec4 Vector1, float a)
    {
        Vec4 newVec4(Vector1.x * a, Vector1.y * a, Vector1.z * a, Vector1.w * a);
        return newVec4;
    }

    inline Vec4 operator/(Vec4 Vector1, int a)
    {
        Vec4 newVec4(Vector1.x / a, Vector1.y / a, Vector1.z / a, Vector1.w / a);
        return newVec4;
    }

    ////////////////////////////////////////////////////////////////////////

    class Mat3
    {
    public:
        float matrix[3][3];
        float matrixArray[9];

        Mat3()
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    matrix[j][i] = 0;

            FillArray();
        }

        Mat3(const float tab[3][3])
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    matrix[j][i] = tab[j][i];

            FillArray();
        }

        inline float detMatrix()
        {
            float det = (this->matrix[0][0] * (this->matrix[1][1] * this->matrix[2][2] - (this->matrix[2][1] * this->matrix[1][2])))
                - (this->matrix[0][1] * (this->matrix[1][0] * this->matrix[2][2] - (this->matrix[2][0] * this->matrix[1][2])))
                + (this->matrix[0][2] * (this->matrix[1][0] * this->matrix[2][1] - (this->matrix[2][0] * this->matrix[1][1])));
            return det;
        }

        void FillArray()
        {
            int i = 0;

            for (int row = 0; row < 3; row++)
            {
                for (int column = 0; column < 3; column++)
                {
                    matrixArray[i] = matrix[column][row];
                    i++;
                }
            }
        }
    };

    ////////////////////////////////////////////////////////////////////////

    class Mat4
    {
    public:
        float matrix[4][4];
        float matrixArray[16];

        Mat4()
        {
            for (int row = 0; row < 4; row++)
                for (int column = 0; column < 4; column++)
                    matrix[column][row] = 0;

            for (int i = 0; i < 16; i++)
                matrixArray[i] = 0;
        }

        Mat4(const float tab[4][4])
        {
            for (int row = 0; row < 4; row++)
                for (int column = 0; column < 4; column++)
                    matrix[row][column] = tab[row][column];

            int i = 0;
            for (int row = 0; row < 4; row++)
            {
                for (int column = 0; column < 4; column++)
                {
                    matrixArray[i] = matrix[column][row];
                    i++;
                }
            }
        }

        inline Mat4 operator*(Mat4 other)
        {
            Mat4 newMat;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    newMat.matrix[i][j] = this->matrix[i][0] * other.matrix[0][j] + this->matrix[i][1] * other.matrix[1][j] + this->matrix[i][2] * other.matrix[2][j] + this->matrix[i][3] * other.matrix[3][j];

            newMat.FillArray();
            return newMat;
        }

        inline Mat4 operator=(Mat4 other)
        {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    this->matrix[i][j] = other.matrix[i][j];

            FillArray();
            return *this;
        }

        inline float detMatrix()
        {
            float matrixA[3][3] = { {this->matrix[1][1], this->matrix[1][2], this->matrix[1][3]},
                                   {this->matrix[2][1], this->matrix[2][2], this->matrix[2][3]},
                                   {this->matrix[3][1], this->matrix[3][2], this->matrix[3][3]} };

            float matrixB[3][3] = { {this->matrix[1][0], this->matrix[1][2], this->matrix[1][3]},
                                   {this->matrix[2][0], this->matrix[2][2], this->matrix[2][3]},
                                   {this->matrix[3][0], this->matrix[3][2], this->matrix[3][3]} };

            float matrixC[3][3] = { {this->matrix[1][0], this->matrix[1][1], this->matrix[1][3]},
                                   {this->matrix[2][0], this->matrix[2][1], this->matrix[2][3]},
                                   {this->matrix[3][0], this->matrix[3][1], this->matrix[3][3]} };

            float matrixD[3][3] = { {this->matrix[1][0], this->matrix[1][1], this->matrix[1][2]},
                                   {this->matrix[2][0], this->matrix[2][1], this->matrix[2][2]},
                                   {this->matrix[3][0], this->matrix[3][1], this->matrix[3][2]} };

            Mat3 detMat4A(matrixA);
            Mat3 detMat4B(matrixB);
            Mat3 detMat4C(matrixC);
            Mat3 detMat4D(matrixD);

            float det = this->matrix[0][0] * detMat4A.detMatrix() - this->matrix[0][1] * detMat4B.detMatrix() + this->matrix[0][2] * detMat4C.detMatrix() - this->matrix[0][3] * detMat4D.detMatrix();
            return det;
        }

        inline static Mat4 CreateTransformMatrix(const Vec3& rotation, const Vec3& position, const Vec3& scale)
        {
            Mat4 transformMatrix = CreateTranslationMatrix(position) * CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z) * CreateScaleMatrix(scale);
            transformMatrix.FillArray();
            return transformMatrix;
        }

        inline static Mat4 CreateTranslationMatrix(const Vec3& translation)
        {
            Mat4 translationMatrix;
            translationMatrix.matrix[0][3] = translation.x;
            translationMatrix.matrix[1][3] = translation.y;
            translationMatrix.matrix[2][3] = translation.z;

            for (int i = 0; i < 4; i++)
                translationMatrix.matrix[i][i] = 1.f;

            translationMatrix.FillArray();
            return translationMatrix;
        }

        inline static Mat4 CreateScaleMatrix(const Vec3& scale)
        {
            Mat4 scaleMat;
            scaleMat.matrix[0][0] = scale.x;
            scaleMat.matrix[1][1] = scale.y;
            scaleMat.matrix[2][2] = scale.z;
            scaleMat.matrix[3][3] = 1.f;

            scaleMat.FillArray();
            return scaleMat;
        }

        inline static Mat4 CreateXRotationMatrix(const float angle) //(Matrix de rotation autour de l’axe des X)
        {
            Mat4 rotaX;
            rotaX.matrix[0][0] = 1.f;
            rotaX.matrix[3][3] = 1.f;

            float radAngle = rad(angle);
            rotaX.matrix[1][1] = cosf(radAngle);
            rotaX.matrix[1][2] = -sinf(radAngle);
            rotaX.matrix[2][1] = sinf(radAngle);
            rotaX.matrix[2][2] = cosf(radAngle);

            rotaX.FillArray();
            return rotaX;
        }

        inline static Mat4 CreateYRotationMatrix(const float angle) //(Matrix de rotation autour de l’axe des Y)
        {
            Mat4 rotaY;
            rotaY.matrix[1][1] = 1.f;
            rotaY.matrix[3][3] = 1.f;

            float radAngle = rad(angle);
            rotaY.matrix[0][0] = cosf(radAngle);
            rotaY.matrix[0][2] = -sinf(radAngle);
            rotaY.matrix[2][0] = sinf(radAngle);
            rotaY.matrix[2][2] = cosf(radAngle);

            rotaY.FillArray();
            return rotaY;
        }

        inline static Mat4 CreateZRotationMatrix(const float angle) //(Matrix de rotation autour de l’axe des Z)
        {
            Mat4 rotaZ;
            rotaZ.matrix[2][2] = 1.f;
            rotaZ.matrix[3][3] = 1.f;

            float radAngle = rad(angle);
            rotaZ.matrix[0][0] = cosf(radAngle);
            rotaZ.matrix[0][1] = -sinf(radAngle);
            rotaZ.matrix[1][0] = sinf(radAngle);
            rotaZ.matrix[1][1] = cosf(radAngle);

            rotaZ.FillArray();
            return rotaZ;
        }

        inline Mat4 transposeMatrix()
        {
            Mat4 newMatrix;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    newMatrix.matrix[j][i] = this->matrix[i][j];

            newMatrix.FillArray();
            return newMatrix;
        }

        inline Mat4 inverseMatrix()
        {
            int line;
            int column;
            float det = this->detMatrix();
            Mat4 inverseMatrix;
            Mat4 adjMatrix;
            Mat3 matrix3;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    line = 0;
                    column = 0;
                    for (int k = 0; k < 3; k++)
                    {
                        for (int h = 0; h < 3; h++)
                        {
                            if (column == j)
                            {
                                column++;
                                if (column >= 4)
                                {
                                    column = 0;
                                    line++;
                                }
                            }

                            if (line == i)
                                line++;

                            matrix3.matrix[k][h] = this->matrix[line][column];
                            column++;

                            if (column >= 4)
                            {
                                column = 0;
                                line++;
                            }
                        }
                    }
                    adjMatrix.matrix[i][j] = powf(-1.f, (float)(i + j)) * matrix3.detMatrix();
                }
            }

            adjMatrix = adjMatrix.transposeMatrix();

            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    inverseMatrix.matrix[i][j] = adjMatrix.matrix[i][j] * (1 / det);

            inverseMatrix.FillArray();
            return inverseMatrix;
        }

        inline static Mat4 identity()
        {
            float matrix[4][4] = {
                {1.f, 0.f, 0.f, 0.f},
                {0.f, 1.f, 0.f, 0.f},
                {0.f, 0.f, 1.f, 0.f},
                {0.f, 0.f, 0.f, 1.f},
            };
            return matrix;
        }

        Mat3 Mat4ToMat3()
        {
            Mat3 mat3;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    mat3.matrix[j][i] = matrix[j][i];

            mat3.FillArray();
            return mat3;
        }

        void FillArray()
        {
            int i = 0;

            for (int row = 0; row < 4; row++)
            {
                for (int column = 0; column < 4; column++)
                {
                    matrixArray[i] = matrix[column][row];
                    i++;
                }
            }
        }
    };

    inline Vec4 operator*(const Mat4 mat4, const Vec4 vec4)
    {
        Vec4 result;
        result.x = mat4.matrix[0][0] * vec4.x + mat4.matrix[0][1] * vec4.y + mat4.matrix[0][2] * vec4.z + mat4.matrix[0][3] * vec4.w;
        result.y = mat4.matrix[1][0] * vec4.x + mat4.matrix[1][1] * vec4.y + mat4.matrix[1][2] * vec4.z + mat4.matrix[1][3] * vec4.w;
        result.z = mat4.matrix[2][0] * vec4.x + mat4.matrix[2][1] * vec4.y + mat4.matrix[2][2] * vec4.z + mat4.matrix[2][3] * vec4.w;
        result.w = mat4.matrix[3][0] * vec4.x + mat4.matrix[3][1] * vec4.y + mat4.matrix[3][2] * vec4.z + mat4.matrix[3][3] * vec4.w;

        return result;
    }
}