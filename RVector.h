#pragma once
#include <DirectXMath.h>
#include <cmath>
#include <vector>

using namespace DirectX;

//XMFLOAT3�����Z�q�I�[�o�[���[�h������RVector3�^
class RVector3 : public XMFLOAT3
{
public:
	//�R���X�g���N�^
	RVector3(float x, float y, float z) : XMFLOAT3(x, y, z) {};
	RVector3() :XMFLOAT3(0, 0, 0) {};

	RVector3 operator+(const RVector3 &other)const { return RVector3(x + other.x, y + other.y, z + other.z); }
	RVector3 operator-(const RVector3 &other)const { return RVector3(x - other.x, y - other.y, z - other.z); }
	RVector3 operator*(const RVector3 &other)const { return RVector3(x * other.x, y * other.y, z * other.z); }
	RVector3 operator/(const RVector3 &other)const { return RVector3(x / other.x, y / other.y, z / other.z); }
	RVector3 &operator+=(const RVector3 &other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	RVector3 &operator-=(const RVector3 &other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	RVector3 &operator*=(const RVector3 &other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	RVector3 &operator/=(const RVector3 &other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }

	RVector3 operator+(const XMFLOAT3 &other)const { return RVector3(x + other.x, y + other.y, z + other.z); }
	RVector3 operator-(const XMFLOAT3 &other)const { return RVector3(x - other.x, y - other.y, z - other.z); }
	RVector3 operator*(const XMFLOAT3 &other)const { return RVector3(x * other.x, y * other.y, z * other.z); }
	RVector3 operator/(const XMFLOAT3 &other)const { return RVector3(x / other.x, y / other.y, z / other.z); }
	RVector3 &operator+=(const XMFLOAT3 &other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	RVector3 &operator-=(const XMFLOAT3 &other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	RVector3 &operator*=(const XMFLOAT3 &other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	RVector3 &operator/=(const XMFLOAT3 &other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }

	RVector3 &operator=(const XMFLOAT3 &other) { 
		RVector3 result = RVector3(other.x, other.y, other.z);
		return result; 
	}
	

	bool operator==(const RVector3 &other) { return this->x == other.x && this->y == other.y && this->z == other.z; }
	bool operator!=(const RVector3 &other) { return !(*this == other); }

	RVector3 operator*(const float &other) { return RVector3(x * other, y * other, z * other); }
	RVector3 operator/(const float &other) { return RVector3(x / other, y / other, z / other); }
	RVector3 operator*=(const float &other) { this->x *= other; this->y *= other; return *this; }
	RVector3 operator/=(const float &other) { this->x /= other; this->y /= other; return *this; }

	float length() { return sqrtf(x * x + y * y + z * z); }
	RVector3 norm() { return RVector3(x / length(), y / length(), z / length()); }
	float distance(const RVector3 &v) { return sqrtf((float)pow(v.x - x, 2) + (float)pow(v.y - y, 2) + (float)pow(v.z - z, 2)); }

	void zero() { x = 0, y = 0, z = 0; }

	RVector3 operator-() const { return RVector3(-x, -y, -z); }

	//XMFLOAT3 operator=(const RVector3 &other) { return XMFLOAT3(other.x, other.y, other.z); }
	//RVector3 operator=(const XMFLOAT3 &other) { return RVector3(other.x, other.y, other.z); }
};

inline float dot(const RVector3 &a, const RVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline RVector3 cross(const RVector3 &a, const RVector3 &b) { return RVector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

inline float distance(const RVector3 &v1,const RVector3 &v2){return sqrtf((float)pow(v1.x - v2.x, 2) + (float)pow(v1.y - v2.y, 2) + (float)pow(v1.z - v2.z, 2));}

inline RVector3 operator*(const float &sum, const RVector3 &sum2) { return RVector3(sum2.x * sum, sum2.y * sum, sum2.z * sum); }
inline RVector3 operator/(const float &sum, const RVector3 &sum2) { return RVector3(sum2.x / sum, sum2.y / sum, sum2.z / sum); }

inline float degree(float vx1, float vy1, float vx2, float vy2) {
	return ((vx1 * vx2) + (vy1 * vy2)) / sqrtf(((float(pow(vx1, 2)) + float(pow(vy1, 2)))) * (float(pow(vx2, 2)) + (float(pow(vy2, 2)))));
}

inline RVector3 degreeRotate(const RVector3& v1, const RVector3& v2) {
	RVector3 result;
	result.x = degree(v1.z, v1.y, v2.z, v2.y);
	result.y = degree(v1.x, v1.z, v2.x, v2.z);
	result.z = degree(v1.x, v1.y, v2.x, v2.y);

	return result;
}

//�ۊ�
inline const RVector3 lerp(const RVector3 &s, const RVector3 &e, const float t) {
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t) + end * t;
}

//���x�A�����x�n

float CalcVelocityToForceAndMass(const float force, const float mass, float nowVel, float &acc);

float CalcAccelToForceAndMass(const float force, const float mass);

float CalcAccelToFramePerVel(float onesecmove);

float CalcGravity(float start, float end, float v0, int time, float& v1);

inline RVector3 CalcTriangleCenter(RVector3 v1, RVector3 v2, RVector3 v3) {

	return RVector3((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3, (v1.z + v2.z + v3.z) / 3);
}

inline RVector3 CalcRVector3Rotate(RVector3& rotateEuler, RVector3& rotateVec) {

	XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotateEuler.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotateEuler.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotateEuler.y));

	XMVECTOR rotVec = {};
	rotVec.m128_f32[0] = rotateVec.x;
	rotVec.m128_f32[1] = rotateVec.y;
	rotVec.m128_f32[2] = rotateVec.z;

}

//RVector3���g�p�����A3�������`���
namespace Rv3Ease {

	enum class RV3_EASE_TYPE
	{
		EASE_LERP,
		EASE_QUAD_IN,
		EASE_QUAD_OUT,
		EASE_QUAD_INOUT,
		EASE_CUBIC_IN,
		EASE_CUBIC_OUT,
		EASE_CUBIC_INOUT,
	};

	class Rv3Easing
	{
	public:
		Rv3Easing() = default;
		~Rv3Easing() = default;

		/// <summary>
		/// �C�[�W���O������
		/// </summary>
		/// <param name="type">�C�[�W���O�̎��</param>
		/// <param name="startPos">�J�n���W</param>
		/// <param name="endPos">�I�����W</param>
		/// <param name="playFrame">�Đ��t���[��</param>
		void Init(RV3_EASE_TYPE type, RVector3 startPos, RVector3 endPos, int playFrame);

		//�C�[�W���O���Đ�
		void Play();

		//���ݍ��W���擾
		RVector3 Update();

		//�C�[�W���O�����Z�b�g
		void Reset();

		const RVector3 GetStart() { return startPos; }
		const RVector3 GetEnd() { return endPos; }
		
		void SetStartPos(const RVector3& pos) { startPos = pos; }
		void SetEndPos(const RVector3& pos) { endPos = pos; }

		RVector3 GetNowpos() { return resultPos; }

		bool IsEnd() { return isEnded; }

	private:
		//�e��C�[�W���O�֐�

		//�ȉ��ϐ�
		RV3_EASE_TYPE	type;
		RVector3		startPos;
		RVector3		endPos;
		int				playFrame;

		int				frame = 0;
		bool			isplay = false;
		RVector3		resultPos;
		bool isEnded	= false;

	};

	class Rv3Spline
	{
	public:
		Rv3Spline() = default;
		~Rv3Spline() = default;

		void Init(RVector3* array, int pCount, int playFrame);

		void Play();

		RVector3 Update();

		void Reset();

	private:
		std::vector<RVector3> splinePoints;
		int frame = 0;
		int playFrame = 0;
		bool isPlay = false;
		RVector3 resultPos;
		int nowPlayIndex = 0;

		RVector3 SplineCurve4(const RVector3& p0, const RVector3& p1, const RVector3& p2, RVector3& p3, float t);
	};

	class Rv3Bezier3
	{
	public:

		Rv3Bezier3() = default;
		~Rv3Bezier3() = default;

		void Init(RVector3 start, RVector3 end, RVector3 ctrlPoint, int playFrame, RV3_EASE_TYPE type);
		
		void Play();

		RVector3 Update();

		void Reset();

		bool isEnded() { return frame >= playFrame; }

		bool isPlaying() { return this->isPlay; }

		RVector3 nowPos() { return resultPos; }

	private:
		
		Rv3Ease::Rv3Easing ease1;
		Rv3Ease::Rv3Easing ease2;
		Rv3Ease::Rv3Easing bezier;

		RVector3 resultPos;

		int frame = 0;
		int playFrame = 0;
		bool isPlay = false;
	};



	/// <summary>
	/// ���`��ԁi�����͊�{�S�����̌`�j
	/// </summary>
	/// <param name="s">�J�n�_</param>
	/// <param name="e">�I���_</param>
	/// <param name="t">�i�s���ԁi1.0 ~ 0.0�j�͈̔�</param>
	/// <returns>RVector3�^��t�̂Ƃ��̍��W��Ԃ�</returns>
	const RVector3 lerp(const RVector3 &s, const RVector3 &e, const float t);

	const RVector3 InQuad(const RVector3 &s, const RVector3 &e, const float t);

	const RVector3 OutQuad(const RVector3 &s, const RVector3 &e, const float t);

	const RVector3 InOutQuad(const RVector3 &s, const RVector3 &e, const float t);

	const RVector3 InCubic(const RVector3& s, const RVector3& e, const float t);

	const RVector3 OutCubic(const RVector3& s, const RVector3& e, const float t);

	const RVector3 InOutCubic(const RVector3& s, const RVector3& e, const float t);


}


//RVector3�^���g�p�����Փ˔���v���~�e�B�u
namespace RV3Colider {



	///�����s�o�E���f�B���O�{�b�N�X AABB
	class Rv3AABB {
	public:
		RVector3 min;	//����T�C�Y
		RVector3 max;	//����T�C�Y
		RVector3 oldPos;//1F�O�̍��W

		//�R���X�g���N�^�A�f�X�g���N�^
		Rv3AABB() = default;
		~Rv3AABB() = default;

		/// <summary>
		/// AABB�f�[�^�̃R���X�g���N�^
		/// </summary>
		/// <param name="min">���S�_����̋���A</param>
		/// <param name="max">���S�_����̋���B</param>
		/// <param name="pos">����̒��S�_</param>
		Rv3AABB(RVector3 min, RVector3 max, RVector3 pos) {
			this->min = pos + min;
			this->max = pos + max;
			this->oldPos = pos;
		}

		/// <summary>
		/// AABB�f�[�^�ݒ�
		/// </summary>
		/// <param name="pos">����̒��S�_</param>
		/// <param name="min">���S�_����̋���A</param>
		/// <param name="max">���S�_����̋���B</param>
		inline void Set(RVector3 pos, RVector3 min, RVector3 max) {
			this->min = pos + min;
			this->max = pos + max;
			this->oldPos = pos;
		}

		/// <summary>
		/// AABB�̃f�[�^�X�V
		/// </summary>
		/// <param name="actpos">���̎��_�ł�AABB�̒��S���W�iRVector3�j</param>
		inline void Update(RVector3 actpos) {
			RVector3 diff = actpos - oldPos;
			min += diff;
			max += diff;
			oldPos = actpos;
		}
	};

	/// <summary>
	/// AABB���m�̔���
	/// </summary>
	/// <param name="box1">�{�b�N�X1</param>
	/// <param name="box2">�{�b�N�X2</param>
	/// <returns>�Փ˔���</returns>
	inline bool ColisionAABB(const Rv3AABB& box1, const Rv3AABB& box2) {
		//��Փ�
		if (box1.min.x > box2.max.x) { return false; }
		if (box1.max.x < box2.min.x) { return false; }
		if (box1.min.y > box2.max.y) { return false; }
		if (box1.max.y < box2.min.y) { return false; }
		if (box1.min.z > box2.max.z) { return false; }
		if (box1.max.z < box2.min.z) { return false; }
		//�Փ�
		return true;
	}

	//��
	class Sphere {
	public:
		//���S���W
		RVector3 center;
		//���a
		float rad;
		//�f�t�H���g�R���X�g���N�^
		Sphere() {
			center = RVector3(0, 0, 0);
			rad = 0;
		}
		//�l�w��R���X�g���N�^
		Sphere(RVector3 pos, float r) {
			center = pos;
			rad = r;
		}
	};

	/// <summary>
	/// �����m�̔���
	/// </summary>
	/// <param name="a">��1</param>
	/// <param name="b">��2</param>
	/// <returns>�Փ˔���</returns>
	inline bool Colision2Sphere(Sphere a, Sphere b) {
		if (a.rad + b.rad < distance(a.center, b.center)) {
			return false;
		}
		//�Փ�
		return true;
	}

	//����
	class Plane
	{
	public:
		//�@���x�N�g��
		RVector3 normal;
		//���_����̋���
		float distance;
		//�f�t�H���g
		Plane() {
			normal.zero();
			distance = 0;
		}
		//�l�w��
		Plane(RVector3 n, float d) {
			normal = n;
			distance = d;
		}
	};

	/// <summary>
	/// ���ƕ��ʂ̏Փ˔���
	/// </summary>
	/// <param name="sphere">���̃v���~�e�B�u</param>
	/// <param name="plane">���ʃv���~�e�B�u</param>
	/// <param name="coliPos">�Փ˓_��Ԃ��ϐ�</param>
	/// <returns>�Փ˔���</returns>
	bool ColisionSphereToPlane(const Sphere& sphere, const Plane& plane, RVector3* coliPos = nullptr);

	class Ray {
	public:
		RVector3 start;
		RVector3 dir;
	};

	//class Segment : public Ray
	//{
	//public:
	//	Segment() = default;
	//	Segment(RVector3 start, RVector3 dir) {
	//		this->start = start;
	//		this->dir = dir;
	//	}
	//	Segment(RVector3 p1, RVector3 p2) {
	//		this->start = p1;
	//		this->end = p2;
	//		this->dir = (p1 - p2).norm();
	//	}

	//	RVector3 end;
	//};

	RVector3 CalcScreen2World(const XMFLOAT2& scrPos, float fz);

	inline Ray CalcScreen2WorldRay(XMFLOAT2& scrPos, float window_w, float window_h, XMMATRIX& prj, XMMATRIX& view) {

		Ray result;
		result.start = CalcScreen2World(scrPos, 0);
		result.dir = CalcScreen2World(scrPos, 1);
		return result;
	}

	inline bool ColisionRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, RVector3* inter = nullptr) {
		const float epsilon = 1.0e-5f;
		RVector3 n = ray.dir;
		n.norm();
		RVector3 pn = plane.normal;
		pn.norm();
		float d1 = dot(pn, n);
		if (d1 > -epsilon) { return false; }
		float d2 = dot(pn, ray.start);
		float dist = d2 - plane.distance;
		float t = dist / -d1;
		if (t < 0) { return false; }
		if (distance) { *distance = t; }
		if (inter) {
			*inter = ray.start + t * n;
		}
		return true;
	}

	bool ColisionRayToAABB(Ray& ray, const Rv3AABB& box, float* distance = nullptr, RVector3* colisionPos = nullptr);

	//�O�p�`
	class Triangle {
	public:
		//���_���W
		RVector3 p0, p1, p2;
		//�@��
		RVector3 normal;

	};

};


#pragma region XMFLOAT_OPERATOR

inline const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs) {
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

inline void operator+=(DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
}

inline const DirectX::XMFLOAT4 operator-(DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs) {
	XMFLOAT4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}

inline const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& lhs, const float a) {
	XMFLOAT4 result;
	result.x = lhs.x / a;
	result.y = lhs.y / a;
	result.z = lhs.z / a;
	result.w = lhs.w / a;
	return result;
}

inline const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lhs, const float a) {
	XMFLOAT4 result;
	result.x = lhs.x * a;
	result.y = lhs.y * a;
	result.z = lhs.z * a;
	result.w = lhs.w * a;
	return result;
}

inline const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
	XMFLOAT4 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;
	return result;
}

inline const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
	XMFLOAT4 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	result.w = a.w * b.w;
	return result;
}

inline const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
	XMFLOAT4 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;
	result.w = a.w / b.w;
	return result;
}


#pragma endregion